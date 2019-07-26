/*
 * gpio.h
 *
 * Created: 01.01.2018 16:03:52
 *  Author: ANGRY_ELECTRONICAN
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#define bool unsigned char
#define TRUE 1
#define FALSE 0

typedef enum {IN=0,OUT=1} DIR;
	
#include <avr/io.h>

void GPIO_setPortDirection(volatile uint8_t *port,uint8_t value);
void GPIO_writePort(volatile uint8_t *port,uint8_t value);
uint8_t GPIO_readPort(volatile uint8_t *port);

void GPIO_setPinDirection(volatile uint8_t *port,uint8_t pin,DIR dir);
void GPIO_writePin(volatile uint8_t *port,uint8_t pin,uint8_t value);
bool GPIO_readPin(volatile uint8_t *port,uint8_t pin);

#endif /* GPIO_H_ */