/*
 * gpio.c
 *
 * Created: 01.01.2018 16:07:17
 *  Author: ANGRY_ELECTRONICAN
 */ 
#include "gpio.h"

void GPIO_setPortDirection(volatile uint8_t *port,uint8_t value){
	*(port-1)=value;
}
void GPIO_writePort(volatile uint8_t *port,uint8_t value){
	(*port)=value;
}
uint8_t GPIO_readPort(volatile uint8_t *port){
	return *(port-2);
}

void GPIO_setPinDirection(volatile uint8_t *port,uint8_t pin,DIR value){
	if(value==OUT){
		*(port-1)|=(1<<pin);
	} else {
		*(port-1)&=~(1<<pin);
	}
}
void GPIO_writePin(volatile uint8_t *port,uint8_t pin,bool value){
	if(value==TRUE){
		*(port)|=(1<<pin);
	}else{
		*(port)&=~(1<<pin);
	}
}
bool GPIO_readPin(volatile uint8_t *port,uint8_t pin){
	return (*(port-2)&(1<<pin))>0;
}