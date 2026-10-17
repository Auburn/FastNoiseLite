! FastNoiseLite conformance driver (Fortran). See conformance/README.md.
! Usage: driver VECTORS_DIR > output.txt
! The module's enum constants differ from the C++ ordinals used in configs.txt, so they are mapped by name.
program driver
  use :: fast_noise_lite
  use, intrinsic :: iso_c_binding
  implicit none

  integer(c_int), parameter :: NOISE_TYPES(0:5) = [FNL_NOISE_OPENSIMPLEX2, FNL_NOISE_OPENSIMPLEX2S, &
    FNL_NOISE_CELLULAR, FNL_NOISE_PERLIN, FNL_NOISE_VALUE_CUBIC, FNL_NOISE_VALUE]
  integer(c_int), parameter :: ROTATION_TYPES(0:2) = [FNL_ROTATION_NONE, FNL_ROTATION_IMPROVE_XY_PLANES, &
    FNL_ROTATION_IMPROVE_XZ_PLANES]
  integer(c_int), parameter :: FRACTAL_TYPES(0:5) = [FNL_FRACTAL_NONE, FNL_FRACTAL_FBM, FNL_FRACTAL_RIDGED, &
    FNL_FRACTAL_PINGPONG, FNL_FRACTAL_DOMAIN_WARP_PROGRESSIVE, FNL_FRACTAL_DOMAIN_WARP_INDEPENDENT]
  integer(c_int), parameter :: DISTANCE_FUNCS(0:3) = [FNL_CELLULAR_DISTANCE_EUCLIDEAN, &
    FNL_CELLULAR_DISTANCE_EUCLIDEANSQ, FNL_CELLULAR_DISTANCE_MANHATTAN, FNL_CELLULAR_DISTANCE_HYBRID]
  integer(c_int), parameter :: RETURN_TYPES(0:6) = [FNL_CELLULAR_RETURN_TYPE_CELLVALUE, &
    FNL_CELLULAR_RETURN_TYPE_DISTANCE, FNL_CELLULAR_RETURN_TYPE_DISTANCE2, FNL_CELLULAR_RETURN_TYPE_DISTANCE2ADD, &
    FNL_CELLULAR_RETURN_TYPE_DISTANCE2SUB, FNL_CELLULAR_RETURN_TYPE_DISTANCE2MUL, FNL_CELLULAR_RETURN_TYPE_DISTANCE2DIV]
  integer(c_int), parameter :: WARP_TYPES(0:2) = [FNL_DOMAIN_WARP_OPENSIMPLEX2, FNL_DOMAIN_WARP_OPENSIMPLEX2_REDUCED, &
    FNL_DOMAIN_WARP_BASICGRID]
  ! One format for every line kind: output stops at the first descriptor without a value.
  character(*), parameter :: FMT = '(A, 2(1X, I0), 3(1X, ES16.8E3))'
  integer, parameter :: MAX_POINTS = 256

  character(4096) :: dir
  real(c_double) :: p2(2, MAX_POINTS), p3(3, MAX_POINTS)
  integer :: n2, n3, u, ios, c
  integer(c_int) :: seed, nt, rot, ft, octaves, cdist, cret, wt
  real(c_float) :: freq, lac, gain, ws, pp, jitter, amp
  type(fnl_state) :: s

  if (command_argument_count() < 1) error stop 'usage: driver VECTORS_DIR'
  call get_command_argument(1, dir)
  call read_points('points2d.txt', p2, n2)
  call read_points('points3d.txt', p3, n3)

  ! fnl_state() is the documented way to create a state (fills in the defaults).
  s = fnl_state()
  call run(s, -1)

  open (newunit=u, file=trim(dir)//'/configs.txt', status='old', action='read')
  c = 0
  do
    read (u, *, iostat=ios) seed, freq, nt, rot, ft, octaves, lac, gain, ws, pp, cdist, cret, jitter, wt, amp
    if (ios /= 0) exit
    s = fnl_state()
    s%seed = seed
    s%frequency = freq
    s%noise_type = NOISE_TYPES(nt)
    s%rotation_type_3d = ROTATION_TYPES(rot)
    s%fractal_type = FRACTAL_TYPES(ft)
    s%octaves = octaves
    s%lacunarity = lac
    s%gain = gain
    s%weighted_strength = ws
    s%ping_pong_strength = pp
    s%cellular_distance_func = DISTANCE_FUNCS(cdist)
    s%cellular_return_type = RETURN_TYPES(cret)
    s%cellular_jitter_mod = jitter
    s%domain_warp_type = WARP_TYPES(wt)
    s%domain_warp_amp = amp
    call run(s, c)
    c = c + 1
  end do
  close (u)

contains

  ! Points are read as double and then rounded to float, like the C++ driver does.
  subroutine read_points(name, p, n)
    character(*), intent(in) :: name
    real(c_double), intent(out) :: p(:, :)
    integer, intent(out) :: n
    integer :: u, ios
    open (newunit=u, file=trim(dir)//'/'//name, status='old', action='read')
    n = 0
    do while (n < size(p, 2))
      read (u, *, iostat=ios) p(:, n + 1)
      if (ios /= 0) exit
      n = n + 1
    end do
    close (u)
  end subroutine read_points

  subroutine run(s, c)
    type(fnl_state), intent(in) :: s
    integer, intent(in) :: c
    real(c_float) :: x, y, z
    integer :: i
    do i = 1, n2
      write (*, FMT) 'N2', c, i - 1, fnl_get_noise_2d(s, real(p2(1, i), c_float), real(p2(2, i), c_float))
    end do
    do i = 1, n3
      write (*, FMT) 'N3', c, i - 1, &
        fnl_get_noise_3d(s, real(p3(1, i), c_float), real(p3(2, i), c_float), real(p3(3, i), c_float))
    end do
    do i = 1, n2
      x = real(p2(1, i), c_float)
      y = real(p2(2, i), c_float)
      call fnl_domain_warp_2d(s, x, y)
      write (*, FMT) 'W2', c, i - 1, x, y
    end do
    do i = 1, n3
      x = real(p3(1, i), c_float)
      y = real(p3(2, i), c_float)
      z = real(p3(3, i), c_float)
      call fnl_domain_warp_3d(s, x, y, z)
      write (*, FMT) 'W3', c, i - 1, x, y, z
    end do
  end subroutine run

end program driver
