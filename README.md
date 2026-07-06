# ps_clone

A minimalistic C++ implementation of the Linux `ps` command that reads process information directly from the `/proc` filesystem instead of relying on external utilities or libraries.

This project was built to understand how process information is exposed by the Linux kernel and how tools like `ps` retrieve and present it.

---

## Features

- Enumerates running processes from `/proc`
- Displays process metadata similar to `ps`
- Extracts process information from:
  - `/proc/[pid]/stat`
  - `/proc/[pid]/status`
  - `/proc/[pid]/cmdline`
- Resolves controlling terminal (TTY) using `tty_nr`
- Maps device numbers to terminal names by traversing `/dev`
- Written entirely in C++

---

## How It Works

1. Iterate through `/proc`.
2. Identify directories whose names are numeric (PIDs).
3. Parse process information from the corresponding files.
4. Extract fields such as:
   - PID
   - Process name
   - Command
   - TTY device number (`tty_nr`)
5. Resolve the terminal name by matching the device number against entries under `/dev`.
6. Format and print the collected information.

---

## Building

### Requirements

- Linux
- C++23 compatible compiler

### Using CMake

```bash
mkdir build
cd build
cmake ..
make
```

### Using g++

```bash
g++ -std=c++23 ps.cpp -o ps
```

---

## Running

```bash
./ps_clone
```

## Concepts Explored

This project explores several Linux internals, including:

- The `/proc` filesystem
- Process metadata
- Parsing `/proc/[pid]/stat`
- Device numbers (`dev_t`)
- Major and minor device numbers
- Character and block devices
- `stat()` system call
- `struct stat`
- `devtmpfs`
- TTY resolution
- Filesystem traversal using `std::filesystem`

---

## Limitations

- Linux only
- Does not implement every option supported by the GNU `ps` command
- Output format is intentionally simplified
- Some kernel-specific edge cases may not be handled

---

## Motivation

The goal of this project was not to recreate every feature of `ps`, but to understand how Linux exposes process information through the `/proc` filesystem and how user-space tools transform that raw data into meaningful output.

---

## Future Improvements

- Support common `ps` flags
- Sort processes by different fields
- Tree view similar to `pstree`
- CPU usage calculation
- Memory usage statistics
- Process filtering
- Colored output
- Unit tests

---

## References

- Linux `proc` documentation
- `man proc`
- `man ps`
- `man stat`
- Linux kernel documentation

---

## License

This project is licensed under the MIT License.