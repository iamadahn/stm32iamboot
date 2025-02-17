cmake_minimum_required(VERSION 3.17)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Add you project name
project(bootloader C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_compile_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
add_compile_options(-lc -lm -lnosys -specs=nano.specs)
add_compile_options(-Wall)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-O0 -g)
endif ()

set(DEFINES
    -DSTM32F103xB
    -DUSE_FULL_LL_DRIVER
    -DFLASH_PAGES=128
    -DFLASH_APP_START_ADDR=0x8000000
    -DFLASH_PAGE_SIZE=1024
)

add_definitions(${DEFINES})

# Add Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/cmsis/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/ll/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys
)

# Add you source file
file(GLOB SOURCES
    "*.s"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/cmsis/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/ll/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/sys/*.c"
)


# Add linker file
file(GLOB LINKER_SCRIPT "*.ld")
message(STATUS "Found linker script - ${LINKER_SCRIPT}")

# this options for C++
add_link_options(-specs=nano.specs -lc -lm)
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

add_subdirectory(${CMAKE_SOURCE_DIR}/src/drivers/gpio)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/drivers/led)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/drivers/usart)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/drivers/flash)
add_subdirectory(${CMAKE_SOURCE_DIR}/iamboot)

target_link_libraries(${PROJECT_NAME}.elf
    gpio_driver
    led_driver
    usart_driver
    flash_driver
    iamboot
)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

add_custom_target(
    build_iamboot_app
    COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR}/iamboot -B ${CMAKE_BINARY_DIR}/iamboot_app -DBUILD_EXECUTABLE=1 -GNinja
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/iamboot_app
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building iamboot app..."
)
