cmake_minimum_required(VERSION 3.15)

project(Masterlator VERSION 0.1)

add_compile_definitions( MASTERLATOR_VERSION="${CMAKE_PROJECT_VERSION}" )

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
