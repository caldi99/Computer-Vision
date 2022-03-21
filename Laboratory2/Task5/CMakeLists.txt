cmake_minimum_required(VERSION 2.8)

project(Task5)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} task5.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})


