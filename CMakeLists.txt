cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

project(blocky)

file(GLOB_RECURSE blocky_SOURCES_HEADERS "src/*.cpp")
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
	#GroupSources(include)
endif(MSVC)

set(blocky_TESTING_SOURCES ${blocky_SOURCES})
list(REMOVE_ITEM blocky_TESTING_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

file(COPY resources DESTINATION .)

include_directories(src)
add_executable(blocky ${blocky_SOURCES_HEADERS})
#add_executable(blocky_test ${blocky_TEST} ${blocky_TESTING_SOURCES})
