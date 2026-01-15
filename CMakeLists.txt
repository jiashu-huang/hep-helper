cmake_minimum_required(VERSION 3.16)

project(hep_helper LANGUAGES CXX)

option(HEP_HELPER_BUILD_TOOLS "Build helper CLI tools" ON)

find_package(ROOT REQUIRED COMPONENTS Core RIO Tree)

add_library(hep_helper
  src/root_fields.cpp
)

target_compile_features(hep_helper PUBLIC cxx_std_17)
target_include_directories(hep_helper PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(hep_helper PUBLIC ROOT::Core ROOT::RIO ROOT::Tree)

if(HEP_HELPER_BUILD_TOOLS)
  add_executable(hep_list_root_fields src/root_fields_main.cpp)
  target_link_libraries(hep_list_root_fields PRIVATE hep_helper)
endif()
