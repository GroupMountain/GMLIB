cmake_minimum_required(VERSION 3.21)

project(GMLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

file(GLOB_RECURSE SRC_FILES_DIR
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.h
)

include_directories(
    ${CMAKE_SOURCE_DIR}/SDK-cpp/include
    ${CMAKE_SOURCE_DIR}/SDK-cpp/include/llapi
    ${CMAKE_SOURCE_DIR}/include
)

link_directories(
    ${CMAKE_SOURCE_DIR}/SDK-cpp/lib
)

add_definitions(
    -D"NDEBUG"
    -D"NOMINMAX"
    -D"TEMPLATE_EXPORTS"
    -D"UNICODE"
    -D"WIN32_LEAN_AND_MEAN"
    -D"_AMD64_"
    -D"_CRT_SECURE_NO_WARNINGS"
    -D"_UNICODE"
    -D"_USRDLL"
    -D"_WINDLL"
    -D"_WINDOWS"
)

add_compile_options(
    /diagnostics:column
    /EHsc
    /FC
    /GL
    /MD
    /nologo
    /permissive-
    /sdl
    /utf-8
    /Zc:inline
    /Zi
    /MP
)

add_link_options(
    /DEBUG
    /DELAYLOAD:"bedrock_server.dll"
    /DLL
    /IGNORE:4199
    /INCREMENTAL:NO
    /LTCG:INCREMENTAL
    /MANIFESTUAC:NO
    /MACHINE:X64
    /NOLOGO
    /OPT:ICF
    /OPT:REF
    /SUBSYSTEM:CONSOLE
)

add_library(${CMAKE_PROJECT_NAME} SHARED ${SRC_FILES_DIR})

target_link_libraries(${CMAKE_PROJECT_NAME} 
	"${CMAKE_SOURCE_DIR}/SDK-cpp/lib/LiteLoader.lib"
	"${CMAKE_SOURCE_DIR}/SDK-cpp/lib/bedrock_server_api.lib"
	"${CMAKE_SOURCE_DIR}/SDK-cpp/lib/bedrock_server_var.lib"
	"${CMAKE_SOURCE_DIR}/SDK-cpp/lib/SymDBHelper.lib"
    "${CMAKE_SOURCE_DIR}/lib/plhook.lib"
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} PRE_BUILD
    COMMAND cmd /c ${PROJECT_SOURCE_DIR}/prepare_libraries.cmd ${PROJECT_SOURCE_DIR}
)