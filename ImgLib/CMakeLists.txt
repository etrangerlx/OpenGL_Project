cmake_minimum_required(VERSION 3.10)
project(ImgLib)

file(GLOB IMGLIB_FILES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/*.h")

add_library(ImgLib STATIC ${IMGLIB_FILES})

target_link_libraries(ImgLib)