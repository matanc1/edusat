cmake_minimum_required(VERSION 3.17)
project(edusat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")


include_directories(.)

add_executable(edusat
        src/edusat.cpp
        src/options.cpp
        )
