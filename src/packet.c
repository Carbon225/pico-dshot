#include <dshot/packet.h>

uint8_t dshot_checksum(uint16_t data)
{
    uint16_t csum = 0;

    for (int i = 0; i < 3; i++)
    {
        csum ^= data;
        data >>= 4;
    }

    return csum & 0xf;
}

dshot_packet_t dshot_throttle_packet(uint16_t throttle, bool telemetry)
{
    dshot_packet_t packet;
    packet.telemetry = telemetry;
    packet.payload = throttle;

    if (packet.payload < DSHOT_THROTTLE_MIN)
        packet.payload = DSHOT_THROTTLE_MIN;

    else if (packet.payload > DSHOT_THROTTLE_MAX)
        packet.payload = DSHOT_THROTTLE_MAX;

    return packet;
}

dshot_packet_t dshot_command_packet(enum dshot_cmd_t cmd)
{
    dshot_packet_t packet;
    packet.telemetry = 1;
    packet.payload = (uint16_t) cmd;
    return packet;
}

uint16_t dshot_encode_packet(dshot_packet_t packet)
{
    uint16_t data = packet.payload;
    data <<= 1;
    data |= packet.telemetry & 1;

    data = (data << 4) | dshot_checksum(data);

    return data;
}
