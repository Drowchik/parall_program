cmake_minimum_required(VERSION 3.11)

project(parall_program LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
    set(CMAKE_MACOSX_RPATH 1)
endif()

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_library(parall_program SHARED
    include/program/program.h
    src/program.cpp
)

target_include_directories(parall_program PUBLIC include)
target_link_libraries(parall_program PUBLIC MPI::MPI_CXX)

add_executable(console_mpi console/console.cpp)
target_link_libraries(console_mpi PRIVATE parall_program MPI::MPI_CXX)

if(APPLE)
    set_target_properties(parall_program PROPERTIES
        MACOSX_RPATH ON
        INSTALL_RPATH "@loader_path"
        BUILD_WITH_INSTALL_RPATH ON
    )
endif()
