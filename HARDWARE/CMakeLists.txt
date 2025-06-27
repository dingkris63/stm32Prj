add_library(hw_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/LED/led.c
    ${CMAKE_CURRENT_SOURCE_DIR}/USART/usart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hwm_init.cpp
)

# Modern CMake prefers target-based includes
target_include_directories(hw_lib
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LED>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/USART>
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(hw_lib PRIVATE os_lib)