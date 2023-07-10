include(${CMAKE_CURRENT_LIST_DIR}/../ImgLib/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/../ImgProcLib/CMakeLists.txt)
cmake_minimum_required(VERSION 3.10)
project(ImgLibTest)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../ImgLib/src)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../ImgProcLib/src)
file(GLOB Test_FILES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

add_executable(ImgLibTest ${Test_FILES})

target_link_libraries(ImgLibTest ImgLib ImgProcLib)