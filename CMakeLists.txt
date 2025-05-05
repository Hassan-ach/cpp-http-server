cmake_minimum_required(VERSION 3.10)
project(HttpServer)

set(CMAKE_CXX_STANDARD 23)

# Include headers
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

add_executable(HttpServer ${SOURCES})

