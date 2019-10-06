cmake_minimum_required(VERSION 3.0)

project(projects CXX)

add_subdirectory (vendor/src/SQLiteCpp)
#add_subdirectory(vendor/src/Pistache/src)

include_directories(vendor/include)
link_directories(vendor/lib)

add_subdirectory(common)
add_subdirectory(bookmanager)
#add_subdirectory(VuePhoto)
#add_subdirectory(todo)
add_subdirectory(translate)

#install(TARGETS projects RUNTIME DESTINATION bin)
