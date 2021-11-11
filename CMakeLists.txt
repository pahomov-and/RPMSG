cmake_minimum_required(VERSION 3.19)
project(thermo_controller C)

set(CMAKE_C_STANDARD 99)

add_executable(${PROJECT_NAME} main.c RPMSG.h RPMSG.c)
target_link_libraries(${PROJECT_NAME} pthread)