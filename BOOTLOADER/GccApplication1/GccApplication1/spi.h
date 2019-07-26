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

#if defined (__AVR_ATmega8__)
#define CE		PB1
#define CSN		PB2
#define MOSI	PB3
#define MISO	PB4
#define SCK		PB5
#endif
#if defined (__AVR_ATMEGA16__)
#define CE		PB3
#define CSN		PB4
#define MOSI	PB5
#define MISO	PB6
#define SCK		PB7
#endif

#define setBit(PORT,PIN) (PORT)|=(1<<PIN)
#define cleBit(PORT,PIN) (PORT)&=~(1<<PIN)

void SPI_Init();
uint8_t SPI_ReadByte(void);
uint8_t SPI_ReadWriteByte(uint8_t data);

#endif /* SPI_H_ */