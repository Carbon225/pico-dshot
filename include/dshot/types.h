#ifndef _DSHOT_TYPES_H_
#define _DSHOT_TYPES_H_

#include <pico/types.h>

#ifdef __cplusplus
extern "C" {
#endif

enum dshot_speed_t
{
    DSHOT150 = 150,
    DSHOT300 = 300,
    DSHOT600 = 600,
    DSHOT1200 = 1200,
};

enum dshot_hardware_t
{
    DSHOT_PWM,
    DSHOT_PIO,
};

typedef struct
{
    uint gpio;
    enum dshot_speed_t speed;
    enum dshot_hardware_t hardware;

    uint dma_chan;
    uint pwm_slice;
    uint pwm_chan;
} dshot_config_t;

static inline dshot_config_t dshot_make_config(uint gpio, enum dshot_speed_t speed, enum dshot_hardware_t hardware)
{
    dshot_config_t c;
    c.gpio = gpio;
    c.speed = speed;
    c.hardware = hardware;
    return c;
}

#ifdef __cplusplus
}
#endif

#endif
