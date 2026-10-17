// FastNoiseLite conformance driver (Zig 0.15). See conformance/README.md.
// Usage: driver f32|f64 VECTORS_DIR > output.txt
// The port is generic over the float type, so one binary holds both precisions. build.sh builds it with
// fastnoise.zig passed as the module "fastnoise". The port's enums use the C++ ordinals.
const std = @import("std");
const fnl = @import("fastnoise");

const Fields = std.mem.TokenIterator(u8, .any);

fn field(f: *Fields) ![]const u8 {
    return f.next() orelse error.ShortLine;
}

fn int(comptime T: type, f: *Fields) !T {
    return std.fmt.parseInt(T, try field(f), 10);
}

// Config values are parsed as f32, like the C++ driver (its setters take float).
fn float(f: *Fields) !f32 {
    return std.fmt.parseFloat(f32, try field(f));
}

fn points(alloc: std.mem.Allocator, dir: std.fs.Dir, name: []const u8, comptime n: usize) ![][n]f64 {
    var list: std.ArrayList([n]f64) = .empty;
    var lines = std.mem.tokenizeAny(u8, try dir.readFileAlloc(alloc, name, 1 << 24), "\r\n");
    while (lines.next()) |line| {
        var f = std.mem.tokenizeAny(u8, line, " \t");
        var p: [n]f64 = undefined;
        for (&p) |*v| v.* = try std.fmt.parseFloat(f64, try field(&f));
        try list.append(alloc, p);
    }
    return list.items;
}

fn eval(comptime F: type, out: *std.Io.Writer, n: fnl.Noise(F), c: i32, p2: []const [2]f64, p3: []const [3]f64) !void {
    for (p2, 0..) |p, i|
        try out.print("N2 {d} {d} {d}\n", .{ c, i, @as(f64, n.genNoise2D(@floatCast(p[0]), @floatCast(p[1]))) });
    for (p3, 0..) |p, i|
        try out.print("N3 {d} {d} {d}\n", .{ c, i, @as(f64, n.genNoise3D(@floatCast(p[0]), @floatCast(p[1]), @floatCast(p[2]))) });
    // No W2/W3 lines: domainWarp2D and domainWarp3D in fastnoise.zig do not compile yet.
    // Flush per instance so that output before a safety panic (ReleaseSafe build) is kept.
    try out.flush();
}

fn run(comptime F: type, alloc: std.mem.Allocator, path: []const u8) !void {
    const dir = try std.fs.cwd().openDir(path, .{});
    const p2 = try points(alloc, dir, "points2d.txt", 2);
    const p3 = try points(alloc, dir, "points3d.txt", 3);
    var buf: [1 << 16]u8 = undefined;
    var stdout = std.fs.File.stdout().writer(&buf);
    const out = &stdout.interface;

    try eval(F, out, .{}, -1, p2, p3);

    var lines = std.mem.tokenizeAny(u8, try dir.readFileAlloc(alloc, "configs.txt", 1 << 24), "\r\n");
    var c: i32 = 0;
    while (lines.next()) |line| : (c += 1) {
        var f = std.mem.tokenizeAny(u8, line, " \t");
        var n: fnl.Noise(F) = .{};
        n.seed = try int(i32, &f);
        n.frequency = try float(&f);
        n.noise_type = @enumFromInt(try int(u8, &f));
        n.rotation_type = @enumFromInt(try int(u8, &f));
        n.fractal_type = @enumFromInt(try int(u8, &f));
        n.octaves = try int(u32, &f);
        n.lacunarity = try float(&f);
        n.gain = try float(&f);
        n.weighted_strength = try float(&f);
        n.ping_pong_strength = try float(&f);
        n.cellular_distance = @enumFromInt(try int(u8, &f));
        n.cellular_return = @enumFromInt(try int(u8, &f));
        n.cellular_jitter_mod = try float(&f);
        n.domain_warp_type = @enumFromInt(try int(u8, &f));
        n.domain_warp_amp = try float(&f);
        try eval(F, out, n, c, p2, p3);
    }
}

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    const alloc = arena.allocator();
    const args = try std.process.argsAlloc(alloc);
    if (args.len != 3 or !(std.mem.eql(u8, args[1], "f32") or std.mem.eql(u8, args[1], "f64"))) {
        std.debug.print("usage: driver f32|f64 VECTORS_DIR\n", .{});
        std.process.exit(2);
    }
    if (std.mem.eql(u8, args[1], "f64")) try run(f64, alloc, args[2]) else try run(f32, alloc, args[2]);
}
