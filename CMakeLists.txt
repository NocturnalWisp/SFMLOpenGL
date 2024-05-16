cmake_minimum_required(VERSION 3.27)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

project(SFMLOpenGL VERSION 0.1 LANGUAGES C CXX)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.0
)
FetchContent_MakeAvailable(SFML)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
    src/gl.c
    src/main.cpp
    src/shader.cpp
    src/texture.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC ${SFML_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)

add_custom_target(
    copy-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_BINARY_DIR}/bin/resources)

add_dependencies(${PROJECT_NAME} copy-files)