/*
 * spi.h
 *
 * Created: 15.09.2017 23:14:09
 *  Author: ANGRY_ELECTRONICAN
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include "gpio.h"

#define CE		PB4
#define CSN		PB0
#define MOSI	PB2
#define MISO	PB3
#define SCK		PB1

#define setBit(PORT,PIN) PORT|=(1<<PIN)
#define cleBit(PORT,PIN) PORT&=~(1<<PIN)

void SPI_Init();
uint8_t SPI_ReadByte(void);
uint8_t SPI_ReadWriteByte(uint8_t data);

#endif /* SPI_H_ */
