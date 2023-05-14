cmake_minimum_required(VERSION 3.25)
project(hw_07)

find_package(Boost COMPONENTS system)
set(CMAKE_CXX_STANDARD 17)

add_executable(hw_07 main.cpp)

target_link_libraries(hw_07 ${Boost_SYSTEM_LIBRARY})

