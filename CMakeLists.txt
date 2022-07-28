cmake_minimum_required(VERSION 3.22.1)

project("JaJaEngine"
        VERSION 1.0
        DESCRIPTION "A C++ game engine made from scratch with SDL2"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

option(MSVC "Microsoft Visual C++" OFF)
option(DEBUG "Debug Mode" ON)

if (!MSVC)
    add_compile_options(
        -O3
        -Wall
        -Wextra
        -Werror
    )

    if (DEBUG)
        add_compile_options(
            -g
        )
    else()
        add_compile_options(
            -s
        )
    endif()
endif()

add_subdirectory(libraries)
add_subdirectory(source)