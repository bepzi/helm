cmake_minimum_required(VERSION 3.15)

project(CONCURRENTQUEUE
  LANGUAGES CXX)

add_library(concurrentqueue INTERFACE)

target_include_directories(concurrentqueue INTERFACE .)

target_compile_features(concurrentqueue INTERFACE cxx_std_11)

set_target_properties(concurrentqueue PROPERTIES CXX_EXTENSIONS OFF)
