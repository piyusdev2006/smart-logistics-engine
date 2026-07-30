cmake_minimum_required(VERSION 3.16)
project(SmartLogisticsEngine LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where to look for header files (.hpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Find Raylib library
find_package(raylib REQUIRED)

# Executable target
add_executable(LogisticsEngine main.cpp)

# Link Raylib and Windows system graphics libraries
target_link_libraries(LogisticsEngine PRIVATE 
    raylib 
    opengl32 
    gdi32 
    winmm
)