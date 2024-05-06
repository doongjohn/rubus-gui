add_executable(rubus-gui-example "")

set_property(TARGET rubus-gui-example PROPERTY EXCLUDE_FROM_ALL true)
set_property(TARGET rubus-gui-example PROPERTY CXX_STANDARD 20)
set_property(TARGET rubus-gui-example PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)

target_sources(
  rubus-gui-example
  PRIVATE
    example/main.cpp
)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(
    rubus-gui-example
    PRIVATE
      -Wall
      -Wextra
  )
endif()

target_link_libraries(
  rubus-gui-example
  PRIVATE
    glfw
    rubus-gui
)

skia_copy_icudtl_dat(rubus-gui-example)

add_subdirectory("example/vendor/glfw-3.4")
set_property(TARGET glfw PROPERTY EXCLUDE_FROM_ALL true)
set_property(TARGET glfw PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
