cmake_minimum_required(VERSION 3.10)
project(myLog VERSION 3.0.0)
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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/myLogApi.cpp
        ${RC_FILES})

if (${PROJECT_NAME_UPPER}_BUILD_SHARED_LIBS)
    if (WIN32 AND NOT MSVC)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif ()
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
    if (MSVC)
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
        if (IS_MULTI_CONFIG)
            target_compile_options(${PROJECT_NAME} PRIVATE
                    $<$<CONFIG:Release>:-O3 -s -fvisibility=hidden -frename-registers -fno-plt  )
            target_link_options(${PROJECT_NAME} PRIVATE
                    $<$<CONFIG:Release>:-Wl,--strip-all)
        else ()
            if (CMAKE_BUILD_TYPE STREQUAL "Release")
                target_compile_options(${PROJECT_NAME} PRIVATE -O3 -s -fvisibility=hidden -frename-registers -fno-plt)
                set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,--strip-all")
            endif ()
        endif ()
    endif ()
else ()
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif ()

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:_DEBUG>)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
        EXPORT_MACRO_NAME ${PROJECT_NAME_UPPER}_API
        EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Api_global.h"
        STATIC_DEFINE ${PROJECT_NAME_UPPER}_STATIC)
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (MINGW)
    target_link_libraries(${PROJECT_NAME} PRIVATE atomic winpthread)
elseif (CYGWIN OR (UNIX AND NOT APPLE))
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif ()

if (UNIX AND NOT APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            INSTALL_RPATH "$ORIGIN"
            CXX_VISIBILITY_PRESET hidden
            VISIBILITY_INLINES_HIDDEN YES)
    target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
endif ()

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

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Api_global.h"
        DESTINATION "include/myLibrary_${TOOLCHAIN_SUFFIX}")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION "include/myLibrary_${TOOLCHAIN_SUFFIX}"
        FILES_MATCHING PATTERN "*.h")

install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION "lib/$<CONFIG>_${TOOLCHAIN_SUFFIX}"
        LIBRARY DESTINATION "lib/$<CONFIG>_${TOOLCHAIN_SUFFIX}"
        RUNTIME DESTINATION "bin/$<CONFIG>_${TOOLCHAIN_SUFFIX}")