#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

#define SPI_DATA_ORDER_LSB_FIRST 1
#define SPI_DATA_ORDER_MSB_FIRST 0
#define SPI_MODE_MASTER 1
#define SPI_MODE_SLAVE 0
#define SPI_CLOCK_POLARITY_HIGH 1
#define SPI_CLOCK_POLARITY_LOW 0
#define SPI_CLOCK_PHASE_SAMPLE_LEADING 0
#define SPI_CLOCK_PHASE_SAMPLE_TRAILING 1

void spi_init(uint8_t, uint8_t, uint8_t, uint8_t);
uint8_t spi_send_byte(uint8_t);

#endif /* SPI_H_ */