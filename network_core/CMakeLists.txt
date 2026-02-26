cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror -fPIC)

project(network_core)

file(GLOB_RECURSE CORE_SOURCES src/*.cpp)

add_library(network_core STATIC ${CORE_SOURCES})

target_include_directories(network_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

target_compile_features(network_core PUBLIC cxx_std_20)
