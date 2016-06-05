 #include <stdint.h>

uint32_t net_checksum_add(int len, uint8_t *buf);
uint16_t net_checksum_finish(uint32_t sum);
uint16_t net_checksum_tcpudp(uint16_t length, uint16_t proto, uint8_t *addrs, uint8_t *buf);
void net_checksum_calculate(uint8_t *data, int length);
