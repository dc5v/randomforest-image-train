cmake_minimum_required(VERSION 3.10)
project(randomforestTrainImage)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(randomforestTrainImage main.cpp)
target_link_libraries(randomforestTrainImage ${OpenCV_LIBS})
