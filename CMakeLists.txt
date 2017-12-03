cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

project(blocky)

file(GLOB_RECURSE blocky_SOURCES "src/*.cpp")
file(GLOB_RECURSE blocky_HEADERS "src/*.hpp")
file(GLOB_RECURSE blocky_TEST "test/test.cpp")

if (MSVC)
	# according to the organization in the file systems
	macro(GroupSources curdir)

		file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir}
		${PROJECT_SOURCE_DIR}/${curdir}/*)

		foreach(child ${children})
			if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
				GroupSources(${curdir}/${child})
			else()
				string(REPLACE "/" "\\" groupname ${curdir})
				source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
			endif()
		endforeach()
	endmacro()

	GroupSources(src)
endif(MSVC)

set(blocky_TESTING_SOURCES ${blocky_SOURCES})
list(REMOVE_ITEM blocky_TESTING_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

file(COPY resources DESTINATION .)

find_package(LibLZMA REQUIRED)
include_directories(${LIBLZMA_INCLUDE_DIR})
set(LIBS ${LIBS} ${LIBLZMA_LIBRARIES})

add_executable(blocky ${blocky_SOURCES} ${blocky_HEADERS})
#add_executable(blocky_test ${blocky_TEST} ${blocky_TESTING_SOURCES})
