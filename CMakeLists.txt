cmake_minimum_required(VERSION 3.16)
project(Image)

# # compiler options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)
set(HEADERS "")
add_subdirectory(modules/psd_sdk)
add_subdirectory(modules/Color)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/psd_sdk/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/Color/src)
add_library(Image src/DecodePsd.cpp src/Export.cpp src/ImageProc.cpp src/Image src/Channel.cpp)

target_link_libraries(Image PUBLIC psd color jpeg png)

add_executable(decode_test test/decode_test.cpp)
target_link_directories(Image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/install/lib)
target_link_libraries(decode_test Image psd color jpeg png)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cli)
add_executable(psd_image_exporter cli/parser.cpp cli/export.cpp)
target_link_directories(Image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/install/lib)
target_link_libraries(psd_image_exporter Image psd color jpeg png)

message(STATUS "PSD_HEADERS: ${PSD_HEADERS}")
message(STATUS "COLOR_HEADERS: ${COLOR_HEADERS}")

file(GLOB cli_inlines ${PROJECT_SOURCE_DIR}/cli/*.inl)
list(APPEND HEADERS ${cli_inlines})
file(GLOB cli_headers ${PROJECT_SOURCE_DIR}/cli/*.h)
list(APPEND HEADERS ${cli_headers})
file(GLOB src_headers ${PROJECT_SOURCE_DIR}/src/*.h)
list(APPEND HEADERS ${src_headers})
file(GLOB src_inlines ${PROJECT_SOURCE_DIR}/src/*.inl)
list(APPEND HEADERS ${src_inlines})

install(TARGETS Image DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(TARGETS color DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(TARGETS psd_image_exporter DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${PSD_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/psd_sdk)
install(FILES ${COLOR_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/color)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
