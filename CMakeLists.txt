cmake_minimum_required(VERSION 3.14)
project(pimpl_on_stack_library)

add_library(pimpl_on_stack STATIC
  src/pimpl_on_stack.cpp)

target_include_directories(pimpl_on_stack PUBLIC include)
