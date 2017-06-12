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

set(rtt_server src/network/RoundTrip_Server.c)
add_executable(RTT_Server
        ${rtt_server}
        include/rdtsc.h)
set(bd_server src/network/PeakBW_Server.c)
add_executable(BD_Server
        ${bd_server}
        include/rdtsc.h)
set(co_server src/network/ConnectOverhead_Server.c)
add_executable(CO_Server
        ${co_server}
        include/rdtsc.h)

set(rtt_client src/network/RoundTrip_Client.c)
add_executable(RTT_Client
        ${rtt_client}
        include/rdtsc.h)
set(bd_client src/network/PeakBW_Client.c)
add_executable(BD_Client
        ${bd_client}
        include/rdtsc.h)
set(co_client src/network/ConnectOverhead_Client.c)
add_executable(CO_Client
        ${co_client}
        include/rdtsc.h)

set(file_cache src/file_system/file_cache.cpp)
add_executable(File_Cache
        ${file_cache}
        include/rdtsc.h)

set(file_read src/file_system/file_read.cpp)
add_executable(File_Read
        ${file_read}
        include/rdtsc.h)

set(file_contension src/file_system/contension.cpp)
add_executable(File_Contension
        ${file_contension}
        include/rdtsc.h)
