# minimum requirement
cmake_minimum_required(VERSION 3.6)

# solution
project(lockExperiment)

SET(TARGET_NAME lockExperiment)
SET(CMAKE_CXX_STANDARD 11)
SET(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


# build TARGET_NAME from source code
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} Threads::Threads)

if(NOT MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif(NOT MSVC)
