project(spi C)
set(make_target "spi")

include(${ROOT_BUILD_PATH}/espf.cmake)

list(APPEND spi_sources
    "src/spi_fsm.c"
    "src/spi_hal.c"
    "src/spi_packet.c"
    "src/spi_thermal.c"
    "src/spi_transaction.c"
)

add_library(${make_target} STATIC
    ${spi_sources}
)

list(
    APPEND module_includes
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/src"
)

target_include_directories(${make_target} PUBLIC ${module_includes} PRIVATE ${ROOT_INCLUDES})
target_compile_definitions(${make_target} PUBLIC ${ROOT_COMPILE_DEFS})
target_compile_options(${make_target} PUBLIC ${ROOT_COMPILE_OPTIONS})
target_link_options(${make_target} PUBLIC ${ROOT_LINK_OPTIONS})
