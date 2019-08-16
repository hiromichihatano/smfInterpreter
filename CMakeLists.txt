cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)
project (smfintp_sample C)

# Define Release by default.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "CMAKE_BUILD_TYPE not specified: Use Release by default.")
endif(NOT CMAKE_BUILD_TYPE)

#
# OUTPUTS
#

# Output Variables
set(OUTPUT_DEBUG Debug/bin)
set(OUTPUT_RELEASE Release/bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${OUTPUT_RELEASE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${OUTPUT_RELEASE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${OUTPUT_RELEASE}")


enable_language(C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_C_EXTENSIONS OFF) #...without compiler extensions like gnu++11
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)


add_executable(smfsample
    ${PROJECT_SOURCE_DIR}/src/smf_dummyCallback.c
    ${PROJECT_SOURCE_DIR}/src/smf_header.c
    ${PROJECT_SOURCE_DIR}/src/smf_lib.c
    ${PROJECT_SOURCE_DIR}/src/smf_midiEvent.c
    ${PROJECT_SOURCE_DIR}/src/smf_trackHeader.c
    ${PROJECT_SOURCE_DIR}/src/smf_debug.c
    ${PROJECT_SOURCE_DIR}/sample/sample.c
)

target_include_directories(smfsample
    PUBLIC ${PROJECT_SOURCE_DIR}/include/
    )
