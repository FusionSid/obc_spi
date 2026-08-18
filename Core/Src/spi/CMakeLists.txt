project(spi C)
set(make_target "spi")

include(${ROOT_BUILD_PATH}/espf.cmake)

list(APPEND spi_sources
    "spi_transaction.c"
    "spi_fsm.c"
    "spi_packet.c"
    "spi_hal.c"
    "queries/spi_thermal_generated.c"
    "queries/spi_radiation_generated.c"
    "queries/spi_radiation_deserialize.c"
    "queries/take_image.c"
    "queries/spi_camera_deserialize.c"
    "queries/spi_camera_generated.c"
    "queries/spi_thermal_deserialize.c"
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

# target_compile_definitions(${make_target}
#     PUBLIC
#     ${ROOT_COMPILE_DEFS}
#     NEW_PACKET_FORMAT
# )

target_compile_definitions(${make_target} PUBLIC ${ROOT_COMPILE_DEFS})
target_compile_options(${make_target} PUBLIC ${ROOT_COMPILE_OPTIONS})
target_link_options(${make_target} PUBLIC ${ROOT_LINK_OPTIONS})
