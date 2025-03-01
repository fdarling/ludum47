# Ludum Dare 47 game

This was our game entry for the "Ludum Dare 47" whose theme is "Stuck in a loop":

https://ldjam.com/events/ludum-dare/47/theme

However, it turned into a longer term project to explore the use of the Box2D library for physics in a custom 2D game engine.

## Dependencies

* [SDL2](https://www.libsdl.org/)
* [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
* [SDL2_gfx](https://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/)
* [Box2D](https://box2d.org/) v2.4 (not v3 yet!)

### Debian / Ubuntu

To install these dependencies under Ubuntu or Debian:

```
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-gfx-dev libbox2d-dev
```

### Windows (MSYS2 &amp; MinGW-w64)

* [MSYS2](https://www.msys2.org/) (faux Linux environment within Windows)
* [MinGW-w64](https://www.mingw-w64.org/) (installed using `pacman` inside of an MSYS2 environment)

You must install GCC, it is best to install it using the `toolchain` group:

```
pacman -S mingw-w64-x86_64-toolchain
```

If you are using CMake instead of the raw Makefile, we need to install `cmake` too:

```
pacman -S mingw-w64-x86_64-cmake
```

SDL2 dependencies:

```
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_gfx
```

MSYS2 has Box2D v3.x in their database, while this project still uses v2.4, so we need to manually download the older package and install it:

```
wget -c https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-box2d-2.4.2-1-any.pkg.tar.zst
pacman -U mingw-w64-x86_64-box2d-2.4.2-1-any.pkg.tar.zst
```

## Compiling

With the raw Makefile:

```
make
```

With CMake:

```
mkdir build
cd build
cmake ..
ninja # if using the "Ninja" build system (the new default, especially on MSYS2)
make # if using the classic "make" build system
```

> [!NOTE]
> on Windows, you must launch the "MSYS2 MinGW 64-bit" terminal, not "MSYS2 MSYS" to set up the environment such that the `cmake` command works properly!

## Running

```
./game
```

## Playing

The controls use WASD:
* <kbd>A</kbd> or <kbd>&#8592;</kbd> for left
* <kbd>D</kbd> or <kbd>&#8594;</kbd> for right
* <kbd>W</kbd> or <kbd>&#8593;</kbd> for jump
* <kbd>S</kbd> or <kbd>&#8595;</kbd> for crouch/climb down (currently not implemented)
* <kbd>SHIFT</kbd> jetpack thruster (experimental)
* <kbd>CTRL</kbd> enable grappling the ceiling (experimental)
* <kbd>ESC</kbd> will exit the game.
