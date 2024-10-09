cmake_minimum_required(VERSION 3.30)

#
# Set the project name
#
project(ccwc)

#
# Set the binary executable name
#
set(project_BIN ${PROJECT_NAME})

#
# Include the updateSubmodules.cmake file
#
# include(../external/updateSubmodules/updateSubmodules.cmake)
# include(CTest)
# add_subdirectory(../external)
# add_subdirectory(test)

#
# Set the C++ standard
#
set(CMAKE_CXX_STANDARD 11)

#
# Set the OS bitness
#
set(OSBitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OSBitness 64)
endif()

#
# Find all executables
#
file(GLOB project_SRCS *.cpp *.h)

#
# Set the output directory
#
set (FullOutputDir "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_SYSTEM_NAME}${OSBitness}/${CMAKE_BUILD_TYPE}")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${FullOutputDir}/static libs")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${FullOutputDir}")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FullOutputDir}")

#
# Add executables to project
#
add_executable(${project_BIN} ${project_SRCS})

