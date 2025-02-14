cmake_minimum_required(VERSION 3.15)

set(SCRS
    ${CMAKE_CURRENT_LIST_DIR}/usart.c
)

add_library(usart_driver INTERFACE)
target_sources(usart_driver INTERFACE ${SCRS})

