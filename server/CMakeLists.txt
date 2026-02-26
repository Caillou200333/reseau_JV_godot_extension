cmake_minimum_required(VERSION 3.16)
project(MyGodotServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

include_directories(includes ../network_core/includes)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(server ${SOURCES})

# 👉 Chemin vers les headers du core
target_include_directories(server PRIVATE
    ${CMAKE_SOURCE_DIR}/../network_core/include
)

# 👉 Chemin vers la librairie compilée
target_link_directories(server PRIVATE
    ${CMAKE_SOURCE_DIR}/../network_core/build
)

# 👉 Lier la librairie
target_link_libraries(server PRIVATE network_core)

set_target_properties(server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
