cmake_minimum_required(VERSION 3.22)

set(SCRS
    ${CMAKE_CURRENT_LIST_DIR}/flash.c
)

add_library(flash_driver INTERFACE)
target_sources(flash_driver INTERFACE ${SCRS})

