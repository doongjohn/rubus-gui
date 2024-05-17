set(SKIA_INC_DIR "$ENV{USERPROFILE}/projects/skia" CACHE PATH "Path to skia directory")
set(SKIA_LIB_DIR "${SKIA_INC_DIR}/out/Debug" CACHE PATH "Path to skia lib directory")

add_library(skia INTERFACE)

target_include_directories(
  skia
  INTERFACE
    ${SKIA_INC_DIR}
)

target_link_libraries(
  skia
  INTERFACE
    ${SKIA_LIB_DIR}/skia.lib
    ${SKIA_LIB_DIR}/skunicode_core.lib
    ${SKIA_LIB_DIR}/skunicode_icu.lib
    ${SKIA_LIB_DIR}/skshaper.lib
    ${SKIA_LIB_DIR}/skparagraph.lib
)

function(skia_copy_icudtl_dat target)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    DEPENDS ${SKIA_LIB_DIR}/icudtl.dat
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${SKIA_LIB_DIR}/icudtl.dat
      $<TARGET_FILE_DIR:${target}>/icudtl.dat
  )
endfunction()
