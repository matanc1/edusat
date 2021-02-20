cmake_minimum_required(VERSION 3.16)
project(edusat)


set(CMAKE_CXX_STANDARD 17)
set(myprogSRCS
        src/main.cpp
        src/heuristics/var.cpp
        src/heuristics/val.cpp
        src/utils/definitions.h
        src/parsing/dimacs_parser.cpp
        src/solver/solver.cpp
        src/callbacks/TimeoutCallback.h
        src/callbacks/CallbackBase.h
        src/solver/solver_state.cpp
        src/callbacks/PrintStateCallback.h
        src/solver/clause.cpp
        src/callbacks/RunStatisticsCallback.h
        src/callbacks/LocalRestart.h
        src/solver/exceptions.h
        )

#set(CMAKE_SYSTEM_NAME Windows)
#set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)
#set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(.)

add_executable(edusat ${myprogSRCS})