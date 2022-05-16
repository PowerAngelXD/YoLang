cmake_minimum_required(VERSION 3.21)

project(yolang)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
message("C++ Standard: ${CMAKE_CXX_STANDRAD}")
file(GLOB SRC_FILES "yolang.cpp", "lexer/lexer.cpp", "parser/*.cpp", "exception/yoexc.cpp", "bytecode/*.cpp", "vm/*.cpp", "console/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES} )