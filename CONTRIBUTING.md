# External dependencies

## Build tools

- [just](https://github.com/casey/just)

## Libraries

- [raylib](https://github.com/raysan5/raylib)

### Header-only

- [raylib-cpp](https://github.com/RobLoach/raylib-cpp)
- [eventpp](https://github.com/wqking/eventpp)

# Build instructions

- Put the header-only libraries into `extern/include/*/`. E.g. `extern/include/eventpp/`
- Build using just, preferably in a container. You could also build manually using the build command inside `Justfile`.

# Coding style

- Adhere to C#'s coding conventions as closely as possible.