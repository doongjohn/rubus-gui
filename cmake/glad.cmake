add_library(glad STATIC "")

set_property(TARGET glad PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)

set(GLAD_DIR "vendor/glad")
target_sources(
  glad
  PRIVATE
    ${GLAD_DIR}/src/glad.c
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      ${GLAD_DIR}/include
    FILES
      ${GLAD_DIR}/include/KHR/khrplatform.h
      ${GLAD_DIR}/include/glad/glad.h
)
