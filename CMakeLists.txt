cmake_minimum_required(VERSION 2.8.4)

project(Masterlator)

find_package(SDL2 REQUIRED)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
else()
   add_compile_definitions( _CRT_SECURE_NO_WARNINGS )
endif()

include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.c)
file(GLOB_RECURSE HEADER_FILES src/*.h)

add_executable(Masterlator ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(Masterlator PUBLIC "src")
target_link_libraries(Masterlator ${SDL2_LIBRARIES})
