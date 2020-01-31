cmake_minimum_required(VERSION 3.0)
project(bookmanager CXX)

set (bookmanager_VERSION_MAJOR 1)
set (bookmanager_VERSION_MINOR 0)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

set (SOURCE_FILES 
   src/Book.cpp
   src/BookController.cpp
   src/BookRepository.cpp
   src/IndexPage.cpp
   src/TokenRepository.cpp
   src/User.cpp
   src/UserController.cpp
   src/UserRepository.cpp
   src/UserServices.cpp
   src/WebServer.cpp
   src/main.cpp)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories (../common)
include_directories(vendor/include)
link_directories(vendor/lib)

add_subdirectory(bookmanager-tests)

add_executable(bookmanager ${SOURCE_FILES})
target_link_libraries(bookmanager common pistache SQLiteCpp sqlite3 pthread dl)

install(TARGETS bookmanager RUNTIME DESTINATION bin)
