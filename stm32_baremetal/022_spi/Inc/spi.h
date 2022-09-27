#ifndef SPI_H_
#define SPI_H_

/*
 * SCK  PB3
 * MISO PB4
 * MOSI PB5
 * SS   PB6
 * AF5
 */

#include <stdint.h>

void SPI1_gpio_init();
void SPI1_config();

void SPI1_enable();
void SPI1_disable();

void SPI1_half_duplex_send(uint8_t *buffer, uint32_t n);
void SPI1_half_duplex_recv(uint8_t *buffer, uint32_t n);

void SPI1_send(uint8_t *buffer, uint32_t n);
void SPI1_recv(uint8_t *buffer, uint32_t n);
void SPI1_transfer(uint8_t *tx_buffer, uint8_t *rx_buffer, uint32_t n);

#endif /* SPI_H_ */
