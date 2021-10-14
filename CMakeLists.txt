cmake_minimum_required(VERSION 3.18.0)

find_package(Git)

if(NOT Git_FOUND)
	message(FATAL_ERROR "could not find Git")
endif()

execute_process(COMMAND git describe --tags --abbrev=0
		RESULT_VARIABLE  _RET
		OUTPUT_VARIABLE  _TAG
		OUTPUT_STRIP_TRAILING_WHITESPACE)

if(_RET)
	message(FATAL_ERROR "failed to get git tag")
endif()

project(cpp-tag-invoke
	LANGUAGES CXX
	VERSION "${_TAG}")

if("^${CMAKE_SOURCE_DIR}$" STREQUAL "^${PROJECT_SOURCE_DIR}$")
	set(IS_MAIN_PROJECT ON)
	message(STATUS "adding cpp-tag-invoke v${_TAG}")
else()
	set(IS_MAIN_PROJECT OFF)
	message(STATUS "adding cpp-tag-invoke v${_TAG} as a sub project")
endif()

add_subdirectory(src)

include(CTest)
add_subdirectory(test EXCLUDE_FROM_ALL)
