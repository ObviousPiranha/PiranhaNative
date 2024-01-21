# PiranhaNative
Native libraries for Jawbone

## Future Options

- https://github.com/nothings/single_file_libs
- https://github.com/mackron/dr_libs

## Build Instructions

For Windows, install [MSYS2](https://www.msys2.org/) and use `pacman` to install `mingw-w64-x86_64-gcc`.

For macOS, install `clang` (Xcode dev tools).

For Linux, install `gcc` or `clang` from the package manager.

Run `make` in the folder `src` to generate `PiranhaNative.dll`.

### Windows Alternative

Install the C++ desktop SDK using the Visual Studio Installer.

Locate and run `vcvars64.bat` to configure shell environment.

Run `build-win32.cmd` in the folder `src` to generate `PiranhaNative.dll`.
