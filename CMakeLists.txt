cmake_minimum_required(VERSION 3.20)
project(PayNix VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enterprise Build Flags for Security and Safety
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall -Wextra -Wpedantic -Werror
        -Wshadow -Wconversion -Wunreachable-code
        -fstack-protector-strong -D_FORTIFY_SOURCE=2
    )
elseif(MSVC)
    add_compile_options(/W4 /WX /sdl /guard:cf)
endif()

# Configuration Types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(include)

add_executable(PayNix 
    src/main.cpp 
    src/Logic.cpp 
    src/UI.cpp
)

# Multi-threading ready for future async operations
find_package(Threads REQUIRED)
target_link_libraries(PayNix Threads::Threads)