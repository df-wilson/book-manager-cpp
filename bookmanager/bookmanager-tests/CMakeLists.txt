#
# Cmake file for test
#

cmake_minimum_required( VERSION 3.0 )

project(bookmanager-tests CXX)
set (CMAKE_CXX_STANDARD 14)

set (SOURCE_FILES
   main.cpp
   ../src/Book.cpp
   ../src/User.cpp
   ../src/BookRepository.cpp
   ../src/BookController.cpp
   ../src/TokenRepository.cpp
   ../src/UserRepository.cpp
   ../src/UserServices.cpp
   ../src/UserController.cpp
   01_dbConnectTest.cpp
   02_UserRepositoryTest.cpp
   03_TokenRepositoryTest.cpp
   04_BookRepositoryTest.cpp
   05_BookController.cpp
   06_UserController.cpp
   07_UserServicesTest.cpp
   )
   
   include_directories (../vendor/include)
   include_directories (../common)
   
add_executable (bookmanager-tests ${SOURCE_FILES})
target_link_libraries(bookmanager-tests common pistache SQLiteCpp sqlite3 pthread dl)
