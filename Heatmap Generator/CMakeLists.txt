cmake_minimum_required(VERSION 2.8)
project(feedAnalyzer)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(feedAnalyzer cvInterface.cpp)
target_link_libraries(feedAnalyzer ${OpenCV_LIBS}) # link OpenCV libraries , hightgui.so not found by cmake so this hack
MESSAGE("OpenCV_LIBS: " ${OpenCV_LIBS} )  #display opencv libs found