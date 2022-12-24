cmake_minimum_required(VERSION 3.16)
project(Image)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)

add_subdirectory(modules/psd_sdk)
add_subdirectory(modules/ColorSpace)

include_directories(modules/psd_sdk/src)
include_directories(modules/ColorSpace/src)
add_library(Image src/DecodePsd.cpp src/JpegExtractor.cpp src/Image)

target_link_libraries(Image PUBLIC psd ColorSpace jpeg)

add_executable(extract_test test/extract.cpp)
target_link_directories(Image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/install/lib)
target_link_libraries(extract_test Image psd ColorSpace jpeg)

