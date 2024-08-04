cmake_minimum_required(VERSION 3.15)

project(obsidain)

set (CMAKE_CXX_STANDARD 20)

set(OBSIDIAN_HEADER_FILES
    inc/colorize.hpp
    src/helper/errors.hpp
    src/helper/flags.hpp
    src/lexer/lexer.hpp
    src/common.hpp
)

set(OBSIDIAN_SOURCE_FILES
    src/main.cpp
    src/lexer/lexer.cpp
)

add_executable(obsidian ${OBSIDIAN_HEADER_FILES} ${OBSIDIAN_SOURCE_FILES})
