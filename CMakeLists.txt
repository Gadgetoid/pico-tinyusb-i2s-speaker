cmake_minimum_required(VERSION 3.12)

set(NAME tinyusb-i2s-audio)
set(FAMILY rp2040)
set(BOARD pico_sdk)

include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)
include(pimoroni_pico_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Include Pimoroni libraries
include(drivers/rgbled/rgbled)

# Add your source files
add_executable(${NAME})

target_sources(${NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/i2s_audio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/usb_descriptors.c
)

target_include_directories(${NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
)

add_compile_definitions(${NAME}
        NDEBUG
        PICO_AUDIO_I2S_DMA_IRQ=0
        PICO_AUDIO_I2S_PIO=0
        PICO_AUDIO_I2S_MONO_OUTPUT=0
        PICO_AUDIO_I2S_MONO_INPUT=0
        PICO_AUDIO_I2S_DATA_PIN=14
        PICO_AUDIO_I2S_CLOCK_PIN_BASE=15
)

target_link_libraries(${NAME} PUBLIC
    pico_stdlib pico_audio_i2s pico_unique_id pico_multicore rgbled tinyusb_device tinyusb_board
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2
    ${CMAKE_CURRENT_LIST_DIR}/README.md
    DESTINATION .
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
