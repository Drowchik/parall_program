cmake_minimum_required(VERSION 3.11)

project(parall_program CXX)

set(CMAKE_CXX_STANDARD 23)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)

    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

add_library(parall_program SHARED "include/program/program.h" "src/program.cpp")
target_include_directories(parall_program PUBLIC "include")


add_executable(console "console/console.cpp")
target_link_libraries(console parall_program)