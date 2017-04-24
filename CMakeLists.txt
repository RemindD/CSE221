cmake_minimum_required (VERSION 2.6)
project (CSE221)
# The version number.
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
include_directories(include)
set(task1 src/CPU.cpp)
add_executable(simple_example
        ${task1}
        include/rdtsc.h)
# add_executable(src/task1.cpp)
# add_executable(Tutorial tutorial.cxx)