cmake_minimum_required(VERSION 3.16)
project(edusat)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
#set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
#set(CMAKE_BUILD_TYPE DEBUG)

include_directories(.)

add_executable(edusat
        src/main.cpp
        src/heuristics/var.cpp
        src/heuristics/val.cpp
        src/utils/definitions.h
        src/parsing/dimacs_parser.cpp
        src/solver/solver.cpp
        src/callbacks/TimeoutCallback.h
        src/callbacks/CallbackBase.h
        src/solver/solver_state.cpp
        src/callbacks/PrintStateCallback.h)