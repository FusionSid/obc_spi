project(spi C)
set(make_target "spi")

include(${ROOT_BUILD_PATH}/espf.cmake)

list(APPEND spi_sources
    "src/spi_transaction.c"
    "src/spi_fsm.c"
    "src/spi_packet.c"
    "src/spi_hal.c"
    "src/queries/spi_thermal_generated.c"
    "src/queries/spi_radiation_generated.c"
    "src/queries/spi_radiation_deserialize.c"
    "src/queries/take_image.c"
    "src/queries/spi_camera_deserialize.c"
    "src/queries/spi_camera_generated.c"
    "src/queries/spi_thermal_deserialize.c"
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

#target_compile_definitions(${make_target}
#    PUBLIC
#     ${ROOT_COMPILE_DEFS}
#    NEW_PACKET_FORMAT
#)

target_compile_definitions(${make_target} PUBLIC ${ROOT_COMPILE_DEFS})
target_compile_options(${make_target} PUBLIC ${ROOT_COMPILE_OPTIONS})
target_link_options(${make_target} PUBLIC ${ROOT_LINK_OPTIONS})
