cmake_minimum_required(VERSION 3.20)
project(RunningSim C)

set(CMAKE_C_STANDARD 11)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        5.5
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
        raylib-aseprite
        GIT_REPOSITORY https://github.com/RobLoach/raylib-aseprite.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(raylib-aseprite)

add_executable(RunningSim main.c)

target_include_directories(RunningSim PRIVATE
        ${raylib-aseprite_SOURCE_DIR}/include
)

target_link_libraries(RunningSim PRIVATE raylib)