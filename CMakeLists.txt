cmake_minimum_required(VERSION 3.16)

project(KruskalsMST LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(KruskalsMST
        main.cpp
        Graph.cpp
        KruskalMST.cpp
        NetworkTopologyParser.cpp
        SimpleExporter.cpp
        Vertex.h
)

target_include_directories(KruskalsMST PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_options(KruskalsMST PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
)

add_custom_command(TARGET KruskalsMST POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:KruskalsMST>/data
)
