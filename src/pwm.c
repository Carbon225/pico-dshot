#include <dshot/pwm.h>
#include <dshot/dshot.h>
#include <dshot/timings.h>
#include <hardware/dma.h>
#include <hardware/pwm.h>
#include <hardware/gpio.h>
#include <hardware/clocks.h>

static inline uint get_dreq_for_slice(uint pwm_slice)
{
    switch (pwm_slice)
    {
        default:
            panic("Invalid PWM slice\n");

        case 0:
            return DREQ_PWM_WRAP0;

        case 1:
            return DREQ_PWM_WRAP1;

        case 2:
            return DREQ_PWM_WRAP2;

        case 3:
            return DREQ_PWM_WRAP3;

        case 4:
            return DREQ_PWM_WRAP4;

        case 5:
            return DREQ_PWM_WRAP5;

        case 6:
            return DREQ_PWM_WRAP6;

        case 7:
            return DREQ_PWM_WRAP7;
    }
}

static void pwm_setup(dshot_config_t *dc)
{
    uint pin = dc->gpio;

    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    uint chan = pwm_gpio_to_channel(pin);

    pwm_config c = pwm_get_default_config();

    float target_freq = (float) dc->speed * 1000.f * (float) DSHOT_TICKS_PER_BIT;
    float div = (float) clock_get_hz(clk_sys) / target_freq;
    pwm_config_set_clkdiv(&c, div);

    pwm_config_set_wrap(&c, DSHOT_TICKS_PER_BIT - 1);
    pwm_init(slice, &c, false);
    pwm_set_chan_level(slice, chan, 0);

    pwm_set_enabled(slice, true);

    dc->pwm_slice = slice;
    dc->pwm_chan = chan;
}

static void dma_setup(dshot_config_t *dc)
{
    uint chan = dma_claim_unused_channel(true);

    dma_channel_config c = dma_channel_get_default_config(chan);

    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    channel_config_set_write_increment(&c, false);
    channel_config_set_read_increment(&c, true);
    channel_config_set_dreq(&c, get_dreq_for_slice(dc->pwm_slice));

    uint16_t *write_addr = (uint16_t*) &pwm_hw->slice[dc->pwm_slice].cc;
    if (dc->pwm_chan == PWM_CHAN_B)
        write_addr += 1;

    dma_channel_configure(
            chan,
            &c,
            write_addr,
            NULL,
            DSHOT_NBITS + 1,
            false
    );

    dc->dma_chan = chan;
}

void dshot_pwm_init(dshot_config_t *c)
{
    pwm_setup(c);
    dma_setup(c);
}

void dshot_pwm_send_raw_blocking(const dshot_config_t *c, uint16_t data)
{
    uint16_t buf[DSHOT_NBITS + 1];
    for (int i = 0; i < DSHOT_NBITS; ++i, data <<= 1)
    {
        if (data & 0x8000)
            buf[i] = DSHOT_T1H;
        else
            buf[i] = DSHOT_T0H;
    }
    buf[DSHOT_NBITS] = 0;

    dma_channel_set_read_addr(c->dma_chan, buf, true);
    dma_channel_wait_for_finish_blocking(c->dma_chan);
}
