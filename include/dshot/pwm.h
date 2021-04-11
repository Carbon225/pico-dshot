#ifndef _DSHOT_PWM_H_
#define _DSHOT_PWM_H_

#include <dshot/dshot.h>

void dshot_pwm_init(dshot_config_t *c);
void dshot_pwm_send_raw_blocking(const dshot_config_t *c, uint16_t data);

#endif
