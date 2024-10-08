cmake_minimum_required(VERSION 3.29)
project(Proyecto1_Scheduling_Ships C)

set(CMAKE_C_STANDARD 23)

add_executable(Proyecto1_Scheduling_Ships main.c
        CEThreads/CEthread.h
        CEThreads/CEthread.c
        CEThreads/CEthread_mutex.h
        CEThreads/CEthread_mutex.c
        CEThreads/CEthread_thread.h
        CEThreads/CEthread_tools.h
        CEThreads/CEthread_tools.c
        CEThreads/main.c
        scheduling/linked_list.h
        scheduling/linked_list.c
        scheduling/schedulers.h
        scheduling/schedulers.c
        channel_control/channel_algorithms.h
        channel_control/channel_algorithms.c
)
