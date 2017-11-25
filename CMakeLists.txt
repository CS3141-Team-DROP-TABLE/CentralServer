cmake_minimum_required (VERSION 2.6)
project (NHM_Central)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)


enable_testing()

include_directories(include)
include_directories(src/lib/include)
include_directories(/usr/include/mariadb)
add_subdirectory(src)

