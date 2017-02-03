cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

project(blocky)

file(GLOB_RECURSE blocky_SOURCES "src/*.cpp")
file(GLOB_RECURSE blocky_HEADERS "include/*.hpp")
file(GLOB_RECURSE blocky_TESTS "test/*.cpp")

file(COPY resources DESTINATION .)

include_directories(include)
add_executable(blocky ${blocky_SOURCES})
target_link_libraries(blocky z)

add_executable(blocky_test ${blocky_TESTS})
