cmake_minimum_required(VERSION 3.20)
project(hydrogen)
set(CMAKE_CXX_STANDARD 20)
add_executable(hydro src/main.cpp
        src/token.hpp
        src/parser.hpp
        src/ast.hpp
        src/generator.hpp
        src/arena.hpp
)

target_include_directories(hydro PUBLIC "${PROJECT_SOURCE_DIR}/src")
