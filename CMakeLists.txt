cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

project(blocky)

file(GLOB_RECURSE blocky_SOURCES "src/*.cpp")
file(GLOB_RECURSE blocky_HEADERS "include/*.hpp")
file(GLOB_RECURSE blocky_TEST "test/test.cpp")

set(blocky_TESTING_SOURCES ${blocky_SOURCES})
list(REMOVE_ITEM blocky_TESTING_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

file(COPY resources DESTINATION .)

include_directories(include)
add_executable(blocky ${blocky_SOURCES})
add_executable(blocky_test ${blocky_TEST} ${blocky_TESTING_SOURCES})
