set(DSHOT_SRCS src/dshot.c src/packet.c src/pwm.c)

add_library(pico_dshot ${DSHOT_SRCS})
target_link_libraries(pico_dshot pico_stdlib hardware_pwm hardware_dma hardware_pio)
target_include_directories(pico_dshot PUBLIC include)
