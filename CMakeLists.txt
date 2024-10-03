cmake_minimum_required(VERSION 3.28)

project(Proyecto1_Scheduling_Ships C)

set(CMAKE_C_STANDARD 23)

add_executable(Proyecto1_Scheduling_Ships
        CEThreads/CEThreads.h
        CEThreads/CEThreads.c
        CEThreads/main.c)

set_target_properties(Proyecto1_Scheduling_Ships PROPERTIES LINKER_LANGUAGE C)

