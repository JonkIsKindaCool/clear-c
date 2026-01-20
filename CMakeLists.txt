cmake_minimum_required(VERSION 4.0.0)
project(clear)

add_executable(clear src/main/main.c)
target_include_directories(clear PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/clear)