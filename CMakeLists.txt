cmake_minimum_required(VERSION 3.29)
project(vkttc)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(SOURCES
    src/main.cpp
        src/jobscheduler.cpp
)

add_executable(main ${SOURCES})
