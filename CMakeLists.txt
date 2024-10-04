cmake_minimum_required(VERSION 3.28)

project(Proyecto1_Scheduling_Ships C)

set(CMAKE_C_STANDARD 23)

add_executable(Proyecto1_Scheduling_Ships
        CEThreads/main.c
        CEThreads/CEthread.c
        CEThreads/CEthread.h
        CEThreads/CEthread_mutex.c
        CEThreads/CEthread_mutex.h
        CEThreads/CEthread_thread.h
        CEThreads/CEthread_tools.c
        CEThreads/CEthread_tools.h
)

set_target_properties(Proyecto1_Scheduling_Ships PROPERTIES LINKER_LANGUAGE C)

