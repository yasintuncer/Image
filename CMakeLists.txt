cmake_minimum_required(VERSION 3.16)


set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)

add_subdirectory(modules/psd_sdk)
add_subdirectory(modules/ColorSpace)
include_directories(".")


