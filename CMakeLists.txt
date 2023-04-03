cmake_minimum_required(VERSION 3.24)
project(python_hw)

set(CMAKE_CXX_STANDARD 17)
add_executable(${PROJECT_NAME} main.cpp library.cpp library.h main.h)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
