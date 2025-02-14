cmake_minimum_required(VERSION 3.22)

set(SCRS
    ${CMAKE_CURRENT_LIST_DIR}/gpio.c
)

add_library(gpio_driver INTERFACE)
target_sources(gpio_driver INTERFACE ${SCRS})

