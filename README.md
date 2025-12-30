# sterm - Simple Terminal

A coroutine-based terminal application written in C++23 using Boost.Cobalt.

## Features

- Asynchronous command execution using Boost.Cobalt coroutines
- Thread-safe output handling
- Command history support
- Configurable via `~/.config/sterm/config`

## Requirements

- C++23 compatible compiler (GCC 13+, Clang 17+, MSVC 19.36+)
- CMake 3.25+
- Boost.Cobalt (via vcpkg or system-installed)

## Installation

### Option 1: Using vcpkg

```bash
# Install dependency
vcpkg install boost-cobalt

# Configure and build
cmake --preset debug
cmake --build --preset debug

# Or for release
cmake --preset release
cmake --build --preset release
```

### Option 2: Using system-installed Boost

If Boost is already installed on your system (e.g., via apt, pacman, brew):

```bash
# Configure and build (no vcpkg required)
cmake --preset system-debug
cmake --build --preset system-debug

# Or for release
cmake --preset system-release
cmake --build --preset system-release
```

### Install

```bash
cmake --install build/release --prefix /usr/local
```

### Building a DEB Package

To create a `.deb` package for Debian/Ubuntu systems:

```bash
# Configure release build
cmake --preset system-release

# Build the project
cmake --build --preset system-release

# Generate the DEB package
cd build/system-release
cpack -G DEB
```

The package will be created in the `packages/` directory with the naming format:
`sterm_<version>_<arch>.deb`

To install the generated package:

```bash
sudo dpkg -i packages/sterm_0.1.0_amd64.deb
```

To uninstall:

```bash
sudo dpkg -r sterm
```

## Configuration

Create the configuration file at `~/.config/sterm/config`:

```
log_file=/path/to/sterm.log
```

## Usage

```bash
./bin/sterm
```

### Available Commands

| Command   | Description                          |
|-----------|--------------------------------------|
| `help`    | Display available commands           |
| `version` | Show program version                 |
| `date`    | Display current date and time        |
| `history` | Show command history                 |
| `quit`    | Exit the program                     |

## License

MIT License - see [LICENSE](LICENSE) for details.
