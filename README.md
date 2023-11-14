# SDL Chess

This is an implementation of chess in C++ using the [SDL library](https://www.libsdl.org).

**TODO IMAGE HERE**

## Building locally

This project uses [CMake](https://cmake.org) and contains vendored copies of [SDL](https://github.com/libsdl-org/SDL)
and [SDL_image](https://github.com/libsdl-org/SDL_image) as submodules.

### Step 1: Setup the repository

```bash
git clone GITHUB_URL_HERE ???
cd ???
git submodule init
```

### Step 2: Build with CMake

#### Visual Studio

1. Open `FOLDER_NAME_HERE` as a CMake project.
2. **Project > Configure ChessNov23**
3. **Build > Build All**

#### Unix

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## TODOS

- Pawn first move
- Castling
- Check/checkmate/stalemate
- Pawn promotion
- En passant
