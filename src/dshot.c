#include <dshot/dshot.h>
#include <dshot/pwm.h>
#include <pico/platform.h>

void dshot_init(dshot_config_t *c)
{
    switch (c->hardware)
    {
        default:
            panic("Not implemented\n");

        case DSHOT_PWM:
            dshot_pwm_init(c);
            break;
    }
}

void dshot_send_raw_blocking(const dshot_config_t *c, uint16_t data)
{
    switch (c->hardware)
    {
        default:
            panic("Not implemented\n");

        case DSHOT_PWM:
            dshot_pwm_send_raw_blocking(c, data);
            break;
    }
}

void dshot_send_blocking(const dshot_config_t *c, dshot_packet_t packet)
{
    dshot_send_raw_blocking(c, dshot_encode_packet(packet));
}
