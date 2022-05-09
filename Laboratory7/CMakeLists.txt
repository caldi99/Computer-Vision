cmake_minimum_required(VERSION 2.8)

project(Lab7)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

set(SOURCES source/main.cpp source/panoramic_utils.cpp source/stiching_utils.cpp include/panoramic_utils.h include/stiching_utils.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})


