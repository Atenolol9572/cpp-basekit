cmake_minimum_required(VERSION 3.10)
project(myLogLib VERSION 2.0.0)
set(PROJECT_NAME_CHINESE "ÈÕÖ¾×«Ð´¿â")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

string(TOUPPER "${PROJECT_NAME}" PROJECT_NAME_UPPER)
option(${PROJECT_NAME_UPPER}_BUILD_SHARED_LIBS "Build shared libraries" OFF)

get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
detect_toolchain_suffix(TOOLCHAIN_SUFFIX)
generate_version_rc(RC_FILES)

set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/myLog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/myLog.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/myLogLib.cpp
        ${RC_FILES})

if (${PROJECT_NAME_UPPER}_BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
    string(REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "/GR-" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    if (IS_MULTI_CONFIG)
        target_compile_options(${PROJECT_NAME} PRIVATE
                $<$<CONFIG:Release>:/O2 /GR- /EHsc>)
        target_link_options(${PROJECT_NAME} PRIVATE
                $<$<CONFIG:Release>:/DEBUG:NONE /OPT:REF /OPT:ICF>)
    else ()
        if (CMAKE_BUILD_TYPE STREQUAL "Release")
            target_compile_options(${PROJECT_NAME} PRIVATE /O2 /GR- /EHsc)
            target_link_options(${PROJECT_NAME} PRIVATE /DEBUG:NONE /OPT:REF /OPT:ICF)
        endif ()
    endif ()
else ()
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        OUTPUT_NAME_DEBUG ${PROJECT_NAME}d
        OUTPUT_NAME_RELEASE ${PROJECT_NAME}
        OUTPUT_NAME_RELWITHDEBINFO ${PROJECT_NAME}
        OUTPUT_NAME_MINSIZEREL ${PROJECT_NAME})

foreach (CONFIG_TYPE Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${CONFIG_TYPE} CONFIG_TYPE_UPPER)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${CMAKE_SOURCE_DIR}/lib/${CONFIG_TYPE}_${TOOLCHAIN_SUFFIX}
            LIBRARY_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${CMAKE_SOURCE_DIR}/lib/${CONFIG_TYPE}_${TOOLCHAIN_SUFFIX}
            RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${CMAKE_SOURCE_DIR}/bin/${CONFIG_TYPE}_${TOOLCHAIN_SUFFIX})
endforeach ()
