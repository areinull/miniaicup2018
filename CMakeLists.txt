cmake_minimum_required(VERSION 3.9)
project(strategy)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb")

set(DEPS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/nlohmann/json.hpp")
set(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_executable(strategy ${SOURCE} ${DEPS_SOURCE})