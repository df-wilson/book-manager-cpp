cmake_minimum_required(VERSION 3.0)

project(projects CXX)

add_subdirectory (vendor/src/SQLiteCpp)

include_directories(vendor/include)
link_directories(vendor/lib)

add_subdirectory(common)
add_subdirectory(bookmanager)
