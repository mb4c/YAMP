cmake_minimum_required(VERSION 3.24)
project(YAMP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")
set(BUILD_TESTING OFF CACHE BOOL "")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libs/taglib/taglib/taglib_config.h.cmake
               ${CMAKE_CURRENT_SOURCE_DIR}/libs/taglib/taglib/taglib_config.h)

add_subdirectory("libs/glfw")
add_subdirectory("libs/taglib")

include_directories(SYSTEM src/
                    libs/glad/include/
                    libs/glfw/include/
                    libs/taglib/taglib/
                    libs/taglib/taglib/toolkit/
                    libs/taglib/taglib/riff/aiff/
                    libs/taglib/taglib/ape/
                    libs/taglib/taglib/asf/
                    libs/taglib/taglib/flac/
                    libs/taglib/taglib/mp4/
                    libs/taglib/taglib/mpc/
                    libs/taglib/taglib/mpeg/
                    libs/taglib/taglib/ogg/
                    libs/taglib/taglib/ogg/opus/
                    libs/taglib/taglib/ogg/speex/
                    libs/taglib/taglib/ogg/vorbis/
                    libs/taglib/taglib/ogg/flac/
                    libs/taglib/taglib/s3m/
                    libs/taglib/taglib/trueaudio/
                    libs/taglib/taglib/wavpack/
                    libs/taglib/taglib/riff/wav/
                    libs/taglib/taglib/mpeg/id3v2/
                    libs/taglib/taglib/mpeg/id3v2/frames/
                    libs/taglib/taglib/mpeg/id3v1/
                    libs/taglib/taglib/riff/
                    libs/taglib/taglib/mod/
                    libs/taglib/taglib/it/
                    libs/taglib/taglib/xm/
                    libs/glm/
                    libs/imgui/
                    libs/json/single_include/nlohmann/
                    libs/miniaudio/)

file(GLOB VENDORS_SOURCES   libs/glad/src/glad.c
                            libs/imgui/*.cpp
                            libs/imgui/backends/imgui_impl_glfw.cpp
                            libs/imgui/backends/imgui_impl_opengl3.cpp
                            libs/imgui/misc/cpp/imgui_stdlib.cpp
                            libs/taglib/taglib/*.cpp)
file(GLOB PROJECT_HEADERS src/*.h src/*.hpp)
file(GLOB PROJECT_SOURCES src/*.cpp)

file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})

target_link_libraries(${PROJECT_NAME} glfw tag
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})

set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(DESTINATION_PATH "${CMAKE_CURRENT_BINARY_DIR}/res")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SOURCE_PATH} ${DESTINATION_PATH})