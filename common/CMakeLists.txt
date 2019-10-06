cmake_minimum_required(VERSION 3.2.2)

set(CMAKE_CXX_FLAGS " -Wall -Wextra -ftabstop=4 -march=native -std=c++14 -fshow-column")

set(SOURCE_FILES 
    ConfigReader.cpp
    Logger.cpp
    dbConnect.cpp
   )

add_library(common SHARED ${SOURCE_FILES}) 
