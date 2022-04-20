cmake_minimum_required(VERSION 2.8)

project(Task1)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

set(SOURCES source/task1.cpp include/Helper.h source/Helper.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})


