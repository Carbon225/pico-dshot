#ifndef _DSHOT_PACKET_H_
#define _DSHOT_PACKET_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// from https://github.com/bitdump/BLHeli/blob/467834db443a887534c1f2c9fb0ff61fd6b40e3e/BLHeli_32%20ARM/BLHeli_32%20Firmware%20specs/Digital_Cmd_Spec.txt
enum dshot_cmd_t
{
    DIGITAL_CMD_MOTOR_STOP,              // Currently not implemented
    DIGITAL_CMD_BEEP1,                  // Wait at least length of beep (260ms) before next command
    DIGITAL_CMD_BEEP2,                  // Wait at least length of beep (260ms) before next command
    DIGITAL_CMD_BEEP3,                  // Wait at least length of beep (280ms) before next command
    DIGITAL_CMD_BEEP4,                  // Wait at least length of beep (280ms) before next command
    DIGITAL_CMD_BEEP5,                  // Wait at least length of beep (1020ms) before next command
    DIGITAL_CMD_ESC_INFO,              // Wait at least 12ms before next command
    DIGITAL_CMD_SPIN_DIRECTION_1,          // Need 6x, no wait required
    DIGITAL_CMD_SPIN_DIRECTION_2,          // Need 6x, no wait required
    DIGITAL_CMD_3D_MODE_OFF,              // Need 6x, no wait required
    DIGITAL_CMD_3D_MODE_ON,              // Need 6x, no wait required
    DIGITAL_CMD_SETTINGS_REQUEST,          // Currently not implemented
    DIGITAL_CMD_SAVE_SETTINGS,              // Need 6x, wait at least 35ms before next command
    DIGITAL_CMD_SPIN_DIRECTION_NORMAL = 20,          // Need 6x, no wait required
    DIGITAL_CMD_SPIN_DIRECTION_REVERSED,          // Need 6x, no wait required
    DIGITAL_CMD_LED0_ON,                  // No wait required
    DIGITAL_CMD_LED1_ON,                  // No wait required
    DIGITAL_CMD_LED2_ON,                  // No wait required
    DIGITAL_CMD_LED3_ON,                  // No wait required
    DIGITAL_CMD_LED0_OFF,              // No wait required
    DIGITAL_CMD_LED1_OFF,              // No wait required
    DIGITAL_CMD_LED2_OFF,              // No wait required
    DIGITAL_CMD_LED3_OFF,              // No wait required
};

#define DSHOT_THROTTLE_MIN (48)
#define DSHOT_THROTTLE_MAX (2047)

typedef struct
{
    uint16_t payload;
    bool telemetry;
} dshot_packet_t;

uint8_t dshot_checksum(uint16_t data);
dshot_packet_t dshot_throttle_packet(uint16_t throttle, bool telemetry);
dshot_packet_t dshot_command_packet(enum dshot_cmd_t cmd);
uint16_t dshot_encode_packet(dshot_packet_t packet);

#ifdef __cplusplus
}
#endif

#endif
