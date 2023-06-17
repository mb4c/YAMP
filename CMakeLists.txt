cmake_minimum_required(VERSION 3.24)
project(YAMP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

find_package(spdlog REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(gtk3 REQUIRED IMPORTED_TARGET gtk+-3.0)
pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0>=1.4)
pkg_search_module(gstreamer-sdp REQUIRED IMPORTED_TARGET gstreamer-sdp-1.0>=1.4)
pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0>=1.4)
pkg_search_module(gstreamer-video REQUIRED IMPORTED_TARGET gstreamer-video-1.0>=1.4)
pkg_search_module(gstreamer-plugins-base REQUIRED IMPORTED_TARGET gstreamer-plugins-base-1.0>=1.4)
pkg_search_module(gstreamer-audio REQUIRED IMPORTED_TARGET gstreamer-audio-1.0>=1.4)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

find_library(TAGLIB_LIBRARY tag)

if(NOT TAGLIB_LIBRARY)
    message(FATAL_ERROR "TagLib library not found")
endif()



include_directories(SYSTEM src/
                    libs/glad/include/
                    ${TAGLIB_INCLUDE_DIRS})

file(GLOB VENDORS_SOURCES   libs/glad/src/glad.c
                            libs/imgui/*.cpp
                            libs/imgui/backends/imgui_impl_glfw.cpp
                            libs/imgui/backends/imgui_impl_opengl3.cpp
                            libs/imgui/misc/cpp/imgui_stdlib.cpp
                            )
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

target_link_libraries(${PROJECT_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                        PkgConfig::gtk3
                        PkgConfig::gstreamer
                        PkgConfig::gstreamer-sdp
                        PkgConfig::gstreamer-app
                        PkgConfig::gstreamer-video
                        PkgConfig::gstreamer-audio
                        PkgConfig::gstreamer-plugins-base
                        spdlog::spdlog
                        nlohmann_json::nlohmann_json
                        imgui::imgui
                        glm::glm
                        glfw
                        ${TAGLIB_LIBRARY})

set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(DESTINATION_PATH "${CMAKE_CURRENT_BINARY_DIR}/res")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SOURCE_PATH} ${DESTINATION_PATH})