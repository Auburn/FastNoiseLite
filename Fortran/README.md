## Getting Started

Here's an example for creating a 128x128 array of OpenSimplex2S noise

```Fortran
program test_fast_noise
  use :: fast_noise_lite
  use, intrinsic :: iso_c_binding
  implicit none

  type(fnl_state) :: noise_state
  real(c_float), dimension(128,128) :: noise_data
  integer(c_int) :: x, y

  ! Create the state.
  noise_state = fnl_state()
  noise_state%noise_type = FNL_NOISE_OPENSIMPLEX2S

  ! Collect the noise data.
  do y = 1,128
    do x = 1,128
      noise_data(x,y) = fnl_get_noise_2d(noise_state, real(x, c_float), real(y, c_float))
    end do
  end do

  ! Do something with this data...
end program test_fast_noise
```