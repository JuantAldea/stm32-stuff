#ifndef I2C_H_
#define I2C_H_
#include <inttypes.h>

void i2c1_init(void);

uint32_t I2C_read_perif(uint8_t perif_addr, uint8_t reg,  uint32_t n, uint8_t *data);

uint32_t I2C_write_perif(uint8_t addr, uint8_t reg, uint32_t n, uint8_t *data, uint8_t send_stop);

uint32_t I2C_write_register(uint8_t addr, uint8_t reg, uint8_t data);

#endif /* I2C_H_ */
