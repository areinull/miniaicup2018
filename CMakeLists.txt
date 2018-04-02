cmake_minimum_required(VERSION 3.9)
project(strategy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb")

set(DEPS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/nlohmann/json.hpp")
set(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
           "${CMAKE_CURRENT_SOURCE_DIR}/src/V2d.cpp"
           "${CMAKE_CURRENT_SOURCE_DIR}/src/V2d.h"
           "${CMAKE_CURRENT_SOURCE_DIR}/src/Field.cpp"
           "${CMAKE_CURRENT_SOURCE_DIR}/src/Field.h"
           "${CMAKE_CURRENT_SOURCE_DIR}/src/Influence.h"
           "${CMAKE_CURRENT_SOURCE_DIR}/src/SimpleGradient.cpp"
           "${CMAKE_CURRENT_SOURCE_DIR}/src/SimpleGradient.h"
           "${CMAKE_CURRENT_SOURCE_DIR}/src/FoodInfluence.cpp"
           "${CMAKE_CURRENT_SOURCE_DIR}/src/FoodInfluence.h"
           "${CMAKE_CURRENT_SOURCE_DIR}/src/RandomInfluence.cpp"
           "${CMAKE_CURRENT_SOURCE_DIR}/src/RandomInfluence.h"
           "${CMAKE_CURRENT_SOURCE_DIR}/src/EnemyInfluence.cpp"
           "${CMAKE_CURRENT_SOURCE_DIR}/src/EnemyInfluence.h")

add_executable(strategy ${SOURCE} ${DEPS_SOURCE})