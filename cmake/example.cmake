add_executable(rubus-gui-example "")

set_property(TARGET rubus-gui-example PROPERTY EXCLUDE_FROM_ALL true)
set_property(TARGET rubus-gui-example PROPERTY CXX_STANDARD 20)

set(USE_ASAN false CACHE BOOL "Use AddressSanitizer")
if (USE_ASAN)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "-O1")
    set_property(TARGET rubus-gui-example PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded)
    set(SANITIZER_OPTS -fno-omit-frame-pointer -fno-sanitize-recover=all -fsanitize=address,undefined)
    target_compile_options(rubus-gui-example PRIVATE ${SANITIZER_OPTS})
    target_link_options(rubus-gui-example PRIVATE ${SANITIZER_OPTS})
  else()
    message(FATAL "USE_ASAN can only be used with clang")
  endif()
else()
  set_property(TARGET rubus-gui-example PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif()

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

add_subdirectory("example/vendor/glfw-3.4")
set_property(TARGET glfw PROPERTY EXCLUDE_FROM_ALL true)

if (USE_ASAN)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "-O1")
    set_property(TARGET glfw PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded)
    set(SANITIZER_OPTS -fno-omit-frame-pointer -fno-sanitize-recover=all -fsanitize=address,undefined)
    target_compile_options(glfw PRIVATE ${SANITIZER_OPTS})
    target_link_options(glfw PRIVATE ${SANITIZER_OPTS})
  else()
    message(FATAL "USE_ASAN can only be used with clang")
  endif()
else()
  set_property(TARGET glfw PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif()

skia_copy_icudtl_dat(rubus-gui-example)
