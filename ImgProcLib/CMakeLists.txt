cmake_minimum_required(VERSION 3.10)
project(ImgProcLib)

file(GLOB IMGPROCLIB_FILES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/*.h")

add_library(ImgProcLib STATIC ${IMGPROCLIB_FILES})

target_link_libraries(ImgProcLib)