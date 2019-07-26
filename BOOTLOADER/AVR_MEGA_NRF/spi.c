/*
 * spi.c
 *
 * Created: 15.09.2017 23:13:55
 *  Author: ANGRY_ELECTRONICAN
 */ 

#include "spi.h"

void SPI_Init(){
	//DDRB=(1<<PB3)|(1<<PB4)|(1<<PB5)|(0<<PB6)|(1<<PB7);
	//PORTB|=(1<<PB4);
	GPIO_setPinDirection(&PORTB,CE,OUT);
	GPIO_setPinDirection(&PORTB,CSN,OUT);
	
	GPIO_setPinDirection(&PORTB,MOSI,OUT);
	GPIO_setPinDirection(&PORTB,MISO,IN);
	GPIO_setPinDirection(&PORTB,SCK,OUT);
	
	GPIO_writePin(&PORTB,CSN,TRUE);
	
	SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
	
}
uint8_t SPI_ReadByte(void){
	while(!(SPSR&(1<<SPIF))){}
		return SPDR;
}
uint8_t SPI_ReadWriteByte(uint8_t data){
	//uint8_t empty=0;
	SPDR=data;
	while(!(SPSR&(1<<SPIF))){}
	return SPDR;	
}