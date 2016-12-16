cmake_minimum_required(VERSION 2.8)

project(coincounter-opencv)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/)

file(GLOB_RECURSE SOURCES "src/*.cpp")
#file(GLOB_RECURSE Foo_HEADERS "src/*.h")

find_package( OpenCV REQUIRED )

include_directories(${PROJECT_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS})

add_executable( coincounter-opencv ${SOURCES})
target_link_libraries( coincounter-opencv ${OpenCV_LIBS} )

