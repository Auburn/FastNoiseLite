// FastNoiseLite conformance driver (Haxe). See conformance/README.md.
// Usage: haxe -cp . -cp .. --run Driver VECTORS_DIR       (eval)
//        haxe -cp . -cp .. -main Driver -js driver.js && node driver.js VECTORS_DIR
// The JS branch uses node's fs/process directly, so no hxnodejs is needed.
import FastNoiseLite;

class Driver {
	static var out = new StringBuf();

	static function read(path:String):String {
		#if js
		return js.Lib.require("fs").readFileSync(path, "utf8");
		#else
		return sys.io.File.getContent(path);
		#end
	}

	static function rows(dir:String, name:String):Array<Array<String>> {
		return [for (l in read(dir + "/" + name).split("\n")) if (StringTools.trim(l) != "") ~/\s+/g.split(StringTools.trim(l))];
	}

	static function points(dir:String, name:String):Array<Array<Float>> {
		return [for (r in rows(dir, name)) [for (f in r) Std.parseFloat(f)]];
	}

	// eval prints infinities as "infinity"/"neg_infinity", which compare.py can't parse.
	static function fmt(v:Float):String {
		return Math.isFinite(v) || Math.isNaN(v) ? Std.string(v) : (v > 0 ? "inf" : "-inf");
	}

	// Each line is built before it is added, so an exception skips just that line.
	static function run(n:Noise, c:Int, p2:Array<Array<Float>>, p3:Array<Array<Float>>) {
		for (i in 0...p2.length) {
			var p = p2[i];
			try out.add('N2 $c $i ${fmt(n.getNoise2D(p[0], p[1]))}\n') catch (e:Dynamic) {}
		}
		for (i in 0...p3.length) {
			var p = p3[i];
			try out.add('N3 $c $i ${fmt(n.getNoise3D(p[0], p[1], p[2]))}\n') catch (e:Dynamic) {}
		}
		for (i in 0...p2.length) {
			var v = new Float2(p2[i][0], p2[i][1]);
			try {
				n.domainWarp2D(v);
				out.add('W2 $c $i ${fmt(v.x)} ${fmt(v.y)}\n');
			} catch (e:Dynamic) {}
		}
		for (i in 0...p3.length) {
			var v = new Float3(p3[i][0], p3[i][1], p3[i][2]);
			try {
				n.domainWarp3D(v);
				out.add('W3 $c $i ${fmt(v.x)} ${fmt(v.y)} ${fmt(v.z)}\n');
			} catch (e:Dynamic) {}
		}
	}

	static function main() {
		#if js
		var dir:String = js.Syntax.code("process.argv[2]");
		#else
		var dir = Sys.args()[0];
		#end
		if (dir == null) throw "usage: Driver VECTORS_DIR";
		var p2 = points(dir, "points2d.txt");
		var p3 = points(dir, "points3d.txt");

		run(new Noise(), -1, p2, p3);
		var c = 0;
		for (r in rows(dir, "configs.txt")) {
			var i = [for (s in r) Std.parseInt(s)];
			var f = [for (s in r) Std.parseFloat(s)];
			var n = new Noise();
			n.seed = i[0];
			n.frequency = f[1];
			// The Haxe enums are declared in the same order as the C++ ones, so ordinals map directly.
			n.noiseType = NoiseType.createByIndex(i[2]);
			n.rotationType3D = RotationType3D.createByIndex(i[3]);
			n.fractalType = FractalType.createByIndex(i[4]);
			n.octaves = i[5];
			n.lacunarity = f[6];
			n.gain = f[7];
			n.weightedStrength = f[8];
			n.pingPongStrength = f[9];
			n.cellularDistanceFunction = CellularDistanceFunction.createByIndex(i[10]);
			n.cellularReturnType = cast i[11]; // enum abstract over Int, values 0..6
			n.cellularJitterModifier = f[12];
			n.domainWarpType = DomainWarpType.createByIndex(i[13]);
			n.domainWarpAmp = f[14];
			run(n, c++, p2, p3);
		}

		#if js
		js.Syntax.code("process.stdout.write({0})", out.toString());
		#else
		Sys.print(out.toString());
		#end
	}
}
