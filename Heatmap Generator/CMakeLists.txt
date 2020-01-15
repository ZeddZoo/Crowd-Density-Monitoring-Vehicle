cmake_minimum_required(VERSION 2.8)
project(feedAnalyzer)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(cvInterface cvInterface.cpp)
target_link_libraries( untitled ${OpenCV_LIBS} /usr/lib/x86_64-linux-gnu/libopencv_highgui.so) # link OpenCV libraries , hightgui.so not found by cmake so this hack
MESSAGE("OpenCV_LIBS: " ${OpenCV_LIBS} )  #display opencv libs found