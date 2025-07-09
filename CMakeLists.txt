cmake_minimum_required(VERSION 3.10)
project(cpp-basekit VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 11)

foreach(CONFIG_TYPE Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${CONFIG_TYPE} CONFIG_TYPE_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CONFIG_TYPE})
endforeach()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(myLog)

add_executable(${PROJECT_NAME}
        src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE myLogLib)
