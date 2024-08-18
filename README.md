# hmah-tetris

## What is this?

Another attempt to write something working in C and OpenGL.

This time we are writing tetris :)

## Build dependencies

* To compile C btw
  
  ![GCC / CLang](https://img.shields.io/badge/GCC_%2F_CLang-blue?logo=gnu&style=for-the-badge)

* To make dependencies
  
  ![CMake ^3.18.0](https://img.shields.io/badge/CMake-%5E3.18.0-green?logo=cmake&style=for-the-badge)

## Build

```shell
mkdir build
cd build
cmake -S .. -B .
cmake --build .
```

## Running

```shell
# In build folder
./tetris.bin
```

## Dependencies

* for matrix transformations

  [![CGLM](https://img.shields.io/badge/CGLM-v0.9.4-green?logo=github&style=for-the-badge)](https://github.com/recp/cglm)

* blazingly fast OpenGL core profile loading

  [![gl3w](https://img.shields.io/badge/gl3w-master-green?logo=github&style=for-the-badge)](https://github.com/skaslev/gl3w)

* for window management, input and sound

  [![SDL](https://img.shields.io/badge/SDL-master-green?logo=github&style=for-the-badge)](https://github.com/libsdl-org/SDL)

* image loading header-only lib (like it, very simple and does its job)

  [![stb_image.h](https://img.shields.io/badge/stb__image.h-master-green?logo=github&style=for-the-badge)](https://github.com/nothings/stb/blob/master/stb_image.h)

## License

[![MIT](https://img.shields.io/badge/MIT-blue?logo=github&style=for-the-badge)](https://opensource.org/license/mit)