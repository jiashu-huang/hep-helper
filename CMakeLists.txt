# Require a modern CMake with target-based properties.
cmake_minimum_required(VERSION 3.16)

# Define the project and enable C++ compilation.
project(hep_helper LANGUAGES CXX)

# Toggle for building optional command-line tools.
option(HEP_HELPER_BUILD_TOOLS "Build helper CLI tools" ON)

# Find the ROOT libraries used by this helper library.
find_package(ROOT REQUIRED COMPONENTS Core RIO Tree)

# Build the core helper library.
add_library(hep_helper
  src/root_fields.cpp
  src/root_event.cpp
)

# Require C++17 and expose headers to dependents.
target_compile_features(hep_helper PUBLIC cxx_std_17)
target_include_directories(hep_helper PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
# Link against the required ROOT components.
target_link_libraries(hep_helper PUBLIC ROOT::Core ROOT::RIO ROOT::Tree)

if(HEP_HELPER_BUILD_TOOLS)
  # Build the CLI tool and link it to the library.
  add_executable(hep_list_root_fields src/root_fields_main.cpp)
  target_link_libraries(hep_list_root_fields PRIVATE hep_helper)

  add_executable(hep_dump_root_event src/root_event_main.cpp)
  target_link_libraries(hep_dump_root_event PRIVATE hep_helper)
endif()
