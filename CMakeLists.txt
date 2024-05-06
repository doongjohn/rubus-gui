cmake_minimum_required(VERSION 3.27)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  RubusGui
  VERSION 0.1.0
  DESCRIPTION ""
  HOMEPAGE_URL ""
)

if (WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always -fansi-escape-codes")
endif()

add_library(rubus-gui "")

set_property(TARGET rubus-gui PROPERTY CXX_STANDARD 20)
set_property(TARGET rubus-gui PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)

target_sources(
  rubus-gui
  PRIVATE
    src/rubus-gui/base.cpp
    src/rubus-gui/screen.cpp
    src/rubus-gui/node_style.cpp
    src/rubus-gui/node.cpp
    src/rubus-gui/tree.cpp
    src/rubus-gui/renderer.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      src
    FILES
      src/rubus-gui/base.hpp
      src/rubus-gui/screen.hpp
      src/rubus-gui/node_style.hpp
      src/rubus-gui/node.hpp
      src/rubus-gui/tree.hpp
      src/rubus-gui/renderer.hpp
)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(
    rubus-gui
    PRIVATE
      -Wall
      -Wextra
  )
endif()

target_link_libraries(
  rubus-gui
  PUBLIC
    opengl32
    glad
    skia
)

include("cmake/glad.cmake")
include("cmake/skia.cmake")
include("cmake/example.cmake")
