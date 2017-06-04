cmake_minimum_required (VERSION 2.6)
project (CSE221)

# configure a header file to pass some of the CMake settings
# to the source code

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")
find_package(Threads)
# add the executable
include_directories(include)
set(task1 src/CPU.cpp)
add_executable(CPU_measure
        ${task1}
        include/rdtsc.h
        include/utilities.h)
target_link_libraries(CPU_measure ${CMAKE_THREAD_LIBS_INIT})
set(task2 src/MEM.cpp)
add_executable(MEM_measure
        ${task2}
        include/rdtsc.h
        include/utilities.h)

set(client src/network/socket_client.c)
add_executable(Client
        ${client}
        include/rdtsc.h)

set(server src/network/socket_server.c)
add_executable(Server
        ${server}
        include/rdtsc.h)

set(file_cache src/file_system/file_cache.cpp)
add_executable(File_Cache
        ${file_cache}
        include/rdtsc.h)

set(file_read src/file_system/file_read.cpp)
add_executable(File_Read
        ${file_read}
        include/rdtsc.h)
