# hmah-tetris

## What is this?

Another attempt to write something working in C and OpenGL.

This time we are writing tetris :)

## Build dependencies

* To compile C btw
  ![GCC / CLang](https://img.shields.io/badge/GCC_%2F_CLang-blue?logo=gnu&style=for-the-badge)
* To make dependencies
  ![CMake ^3.16.0](https://img.shields.io/badge/CMake-%5E3.16.0-green?logo=cmake&style=for-the-badge)
* To download `stb_image.h`
  ![Curl](https://img.shields.io/badge/Curl-blue?logo=gnu&style=for-the-badge) 
* For shaders build tool
  ![Mono](https://img.shields.io/badge/Mono-blue?logo=dotnet&style=for-the-badge)
> Current version can't update `src/shaders/<shader>.c`
>
> If you want to update them, do it manually by copying compiled `src/shaders/compiled/<shader>_vs.h` and `src/shaders/compiled/<shader>_fs.h` into `src/shaders/<shader>.c`
>
> Why? Because the symbols from the headers intersect when linking different `*.o` objects

## Build

> Current version only supports posix OS (linux, freebsd, maybe MacOS)

```shell
make deps # Build dependencies
make # Build tetris
```

## Running

```shell
make run
```

## Clean

```shell
make clean
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