cmake_minimum_required(VERSION 3.25)
project(thread_hw LANGUAGES CXX)

find_package(Threads REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
