#ifndef _DSHOT_DSHOT_H_
#define _DSHOT_DSHOT_H_

#include <dshot/packet.h>
#include <dshot/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void dshot_init(dshot_config_t *c);
void dshot_send_raw_blocking(const dshot_config_t *c, uint16_t data);
void dshot_send_blocking(const dshot_config_t *c, dshot_packet_t packet);

#ifdef __cplusplus
}
#endif

#endif
