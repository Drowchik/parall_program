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

find_package(OpenMP REQUIRED)


add_library(parall_program SHARED "include/program/program.h" "src/program.cpp")
target_include_directories(parall_program PUBLIC "include")

if(OpenMP_CXX_FOUND)
    target_link_libraries(parall_program PRIVATE OpenMP::OpenMP_CXX)
endif()


add_executable(console "console/console.cpp")
target_link_libraries(console PRIVATE parall_program)


if(OpenMP_CXX_FOUND)
    target_link_libraries(console PRIVATE OpenMP::OpenMP_CXX)
endif()