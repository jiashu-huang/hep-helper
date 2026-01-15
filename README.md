# HEP Helper

Author: Jiashu Huang (Brown U)

I often find myself going back to a lot of basic functions when doing high-energy physics analysis. This is why I build a dedicated repo for these helper functions. I then get to use this as a submodule in my analyses.

This repo is a small C++ helper library intended to be linked from analysis repos.

## Build

This repo uses CMake and depends on ROOT (Core, RIO, Tree).

From a parent CMake project:

```cmake
add_subdirectory(hep-helper)
target_link_libraries(your_target PRIVATE hep_helper)
```

To build CLI tools (optional):

```sh
cmake -DHEP_HELPER_BUILD_TOOLS=ON -S . -B build
cmake --build build
```

## Getting Started

This repo uses CMake. If ROOT is available in your environment, the following
commands configure and build the library and the CLI tools:

```sh
cmake -S . -B build
cmake --build build
```

The tools are built by default (`HEP_HELPER_BUILD_TOOLS=ON`). If you only want
the library, configure with `-DHEP_HELPER_BUILD_TOOLS=OFF`.

## Using as a Submodule

Add the repo as a git submodule inside your analysis project:

```sh
git submodule add <repo-url> hep-helper
git submodule update --init --recursive
```

Then wire it into your analysis CMake project:

```cmake
add_subdirectory(hep-helper)
target_link_libraries(your_target PRIVATE hep_helper)
```

Make sure the ROOT environment is set up before configuring your parent
project so CMake can find `ROOT`.

## Tools

* `hep_list_root_fields`: list branch names and types for a ROOT TTree.
  Usage: `hep_list_root_fields file.root output.csv [tree_name]`
  Output: CSV with columns `tree`, `branch`, `type` (tree_name defaults to `Events`).
