/*
 * BLINK.c
 *
 * Created: 08.02.2018 21:37:37
 * Author : ANGRY_ELECTRONICAN
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

int main(void)
{
	void(*boot)(void)=0x1800;
	DDRC|=(1<<PB5);
	PORTC&=~(1<<PC5);
    while (1) 
    {
		//DDRB|=(1<<PB5);
		
		for(int i=0;i<4;i++){
			for(int j=0;j<50*2;j++){
				PORTC^=(1<<PB5);
				_delay_ms(1);
			}
			_delay_ms(1000);
		}
		
		
		for (int i=0;i<200*2;i++){
			PORTC^=(1<<PB5);
			_delay_ms(1);
			
		}
		_delay_ms(1000);

		//PORTC^=(1<<PB5);
		//_delay_ms(1);
		boot();
}
}
