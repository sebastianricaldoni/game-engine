# First SDL Window

This is the first step of a small game engine: create a native window, process
operating-system events, and shut down cleanly.

## Install SDL3 (once)

```sh
brew install sdl3
```

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Run

```sh
./build/first_window
```

Close the window normally or press Escape to exit.
