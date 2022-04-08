cmake_minimum_required(VERSION 2.8)

project(Task1)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

set(SOURCES source/task1.cpp include/HandlerTrackbars.h source/HandlerTrackbars.cpp)

set(CMAKE_BUILD_TYPE RelWithDebInfo)


add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})


