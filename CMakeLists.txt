cmake_minimum_required(VERSION 3.10)
project(cpp-basekit VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)

include(${CMAKE_SOURCE_DIR}/cmake/buildSupport.cmake)
detect_toolchain_suffix(TOOLCHAIN_SUFFIX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/$<CONFIG>_${TOOLCHAIN_SUFFIX}")

add_subdirectory(myLog)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE myLog)
