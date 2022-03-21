cmake_minimum_required(VERSION 2.8)

project(Task2)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

set(SOURCES task2.cpp filters.cpp filters.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})


