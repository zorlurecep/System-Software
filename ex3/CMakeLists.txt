cmake_minimum_required(VERSION 3.22)
project(lab5 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(lib)

add_executable(lab5
        lib/dplist.h
        lib/dplist.c
        config.h
        datamgr.c
        datamgr.h
        main.c)

#add_executable(lab5
#        file_creator.c)

add_compile_definitions(SET_MAX_TEMP=27.96)
add_compile_definitions(SET_MIN_TEMP=14.01)
add_compile_definitions(DEBUG)

#target_link_libraries(lab5 /lib/libdplist.so)
