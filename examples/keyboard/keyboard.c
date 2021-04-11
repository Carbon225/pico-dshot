#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/dshot.h>
#include <dshot/timings.h>

#define REPEAT_FOR(us, fn)                                              \
({                                                                      \
    for (uint64_t __start = time_us_64(); time_us_64() < __start + us;) \
        fn;                                                             \
})

#define DSHOT_GPIO (15)

enum program_mode_t
{
    PROGRAM_IDLE,
    PROGRAM_ARM,
    PROGRAM_SLOW,
};

int main()
{
    stdio_init_all();

    dshot_config_t dshot = dshot_make_config(DSHOT_GPIO, DSHOT300, DSHOT_PWM);
    dshot_init(&dshot);

    getchar();
    puts("Commands:\n"
         "i - idle (no pulses)\n"
         "a - idle (armed)\n"
         "b - beep\n"
         "t - throttle up and down\n"
         "r - set direction reversed\n"
         "n - set normal direction"
         "s - slow spin\n"
         "q - quit");

    enum program_mode_t mode = PROGRAM_IDLE;
    int cmd;
    do
    {
        cmd = getchar_timeout_us(0);
        if (cmd > 0)
        {
            switch (cmd)
            {
                case 'a':
                    puts("Idle (armed)");
                    mode = PROGRAM_ARM;
                    break;

                case 'q':
                    puts("Quit");
                default:
                case 'i':
                    puts("Going idle");
                    mode = PROGRAM_IDLE;
                    break;

                case 'b':
                    puts("Beep");
                    dshot_send_blocking(&dshot, dshot_command_packet(DIGITAL_CMD_BEEP1));
                    sleep_ms(280);
                    break;

                case 's':
                    puts("Slow spin");
                    mode = PROGRAM_SLOW;
                    break;

                case 'r':
                    puts("Setting direction reversed");
                    for (int i = 0; i < 10; ++i)
                    {
                        dshot_send_blocking(&dshot, dshot_command_packet(DIGITAL_CMD_SPIN_DIRECTION_REVERSED));
                        sleep_us(DSHOT_PAUSE_US);
                    }
                    break;

                case 'n':
                    puts("Setting direction normal");
                    for (int i = 0; i < 10; ++i)
                    {
                        dshot_send_blocking(&dshot, dshot_command_packet(DIGITAL_CMD_SPIN_DIRECTION_NORMAL));
                        sleep_us(DSHOT_PAUSE_US);
                    }
                    break;

                case 't':
                    puts("Speeding up");
                    for (int speed = DSHOT_THROTTLE_MIN; speed <= 1000; speed += 1)
                    {
                        REPEAT_FOR(1e3, {
                            dshot_send_blocking(&dshot, dshot_throttle_packet(speed, false));
                            sleep_us(DSHOT_PAUSE_US);
                        });
                    }

                    puts("Slowing down");
                    for (int speed = 1000; speed >= DSHOT_THROTTLE_MIN; speed -= 1)
                    {
                        REPEAT_FOR(1e3, {
                            dshot_send_blocking(&dshot, dshot_throttle_packet(speed, false));
                            sleep_us(DSHOT_PAUSE_US);
                        });
                    }
                    break;
            }
        }

        switch (mode)
        {
            default:
            case PROGRAM_IDLE:
                break;

            case PROGRAM_ARM:
                dshot_send_blocking(&dshot, dshot_throttle_packet(DSHOT_THROTTLE_MIN, false));
                break;

            case PROGRAM_SLOW:
                dshot_send_blocking(&dshot, dshot_throttle_packet(100, false));
                break;
        }

        sleep_us(DSHOT_PAUSE_US);

    } while (cmd != 'q');

    return 0;
}
