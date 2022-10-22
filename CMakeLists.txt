cmake_minimum_required(VERSION 3.21)

project(yolang)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
message("C++ Standard: ${CMAKE_CXX_STANDARD}")
file(GLOB SRC_FILES "yolang.cpp", "lexer/lexer.cpp",
        "parser/*.cpp", "exception/yoexc.cpp",
        "bytecode/*.cpp", "offical_ysh/ysh.cpp"
        "console/*.cpp", "yvm/core/type/*.cpp",
        "yvm/core/storage/*.cpp", "yvm/core/yvm.cpp", "yvm/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES} )
