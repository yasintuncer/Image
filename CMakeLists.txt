cmake_minimum_required(VERSION 3.16)
project(Image)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)

add_subdirectory(modules/psd_sdk)
add_subdirectory(modules/Color)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/psd_sdk/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/Color/src)
add_library(Image src/DecodePsd.cpp src/Export.cpp src/ImageProc.cpp src/Image src/Channel.cpp)

target_link_libraries(Image PUBLIC psd color jpeg png)

add_executable(extract_test test/extract.cpp)
target_link_directories(Image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/install/lib)
target_link_libraries(extract_test Image psd color jpeg png)

add_executable(decode_test test/decode_test.cpp)
target_link_directories(Image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/install/lib)
target_link_libraries(decode_test Image psd color jpeg png)

