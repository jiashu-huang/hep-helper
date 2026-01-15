# HEP Helper

Author: Jiashu Huang (Brown U)

A small C++ helper library for high-energy physics analyses. The repo is meant
to be added as a submodule and linked from multiple analysis projects.

## Quick Start

Requirements:

- ROOT with Core, RIO, Tree components available in your environment
- CMake 3.16+ and a C++17 compiler

Build the library and tools:

```sh
cmake -S . -B build
cmake --build build
```

The tool binaries are produced in `build/` (for example,
`build/hep_list_root_fields`).

Tool usage and source layout are documented in `src/README.md`.

## Using as a Submodule

Add the repo:

```sh
git submodule add <repo-url> hep-helper
git submodule update --init --recursive
```

Wire into your parent CMake project:

```cmake
add_subdirectory(hep-helper)
target_link_libraries(your_target PRIVATE hep_helper)
```

Make sure ROOT is available before configuring your parent project so CMake
can find `ROOT`.

## Library API

Header: `include/hep_helper/root_fields.h`

```cpp
void WriteRootFieldsCsv(const std::string& file_path,
                        const std::string& output_path,
                        const std::string& tree_name = "Events");
```

Header: `include/hep_helper/root_event.h`

```cpp
void WriteFirstRootEventTxt(const std::string& file_path,
                            const std::string& output_path,
                            const std::string& tree_name = "Events");
```

## Optional: Build Settings

To disable building CLI tools (library only):

```sh
cmake -DHEP_HELPER_BUILD_TOOLS=OFF -S . -B build
cmake --build build
```

## Troubleshooting

- CMake cannot find ROOT: ensure your ROOT environment is sourced before
  running CMake.
