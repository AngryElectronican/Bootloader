/*
 * GccApplication1.c
 *
 * Created: 05.02.2018 0:35:22
 * Author : ANGRY_ELECTRONICAN
 */ 

#include <avr/io.h>
#include <avr/boot.h>
#define F_CPU 8000000UL
#include <util/delay.h>

#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "NRF.h"
#include <avr/wdt.h>

#define CE		PB1
#define CSN		PB2
#define MOSI	PB3
#define MISO	PB4
#define SCK		PB5

#define setBit(PORT,PIN) (PORT)|=(1<<PIN)
#define cleBit(PORT,PIN) (PORT)&=~(1<<PIN)

void boot_program_page (uint32_t page, uint8_t *buf);
uint8_t memory[64]={0};
	/*uint8_t ALL_MEM[96]={0x12,0xC0,0x19,0xC0,0x18,0xC0,0x17,0xC0,0x16,0xC0,0x15,0xC0,0x14,0xC0,0x13,0xC0,0x12,0xC0,0x11,0xC0,0x10,0xC0,0x0F,0xC0,0x0E,0xC0,0x0D,0xC0,0x0C,0xC0,0x0B,0xC0,
	0x0A,0xC0,0x09,0xC0,0x08,0xC0,0x11,0x24,0x1F,0xBE,0xCF,0xE5,0xD4,0xE0,0xDE,0xBF,0xCD,0xBF,0x02,0xD0,0x13,0xC0,0xE4,0xCF,0x80,0xE2,0x87,0xBB,0x14,0xBA,0x85,0xBB,
	0x90,0xE2,0x88,0xB3,0x89,0x27,0x88,0xBB,0x2F,0xEF,0x30,0xE7,0x82,0xE0,0x21,0x50,0x30,0x40,0x80,0x40,0xE1,0xF7,0x00,0xC0,0x00,0x00,0xF3,0xCF,0xF8,0x94,0xFF,0xCF
		};*/
	
int main(void)
{
	uint8_t state_machine=0;
	uint8_t buf=0;
	uint8_t pause=0;
	uint8_t num_of_pages=0,page=0;
	uint8_t counter=0;
	void (*start)( void ) = 0x0000;
	void (*boot)( void ) = 0x1800;
	uint8_t pipe0[]={33,33,33,33,33};
	uint8_t TX0[]={33,33,33,33,33};
		
	//uint8_t pipe1[]={66,33,33,33,33};
	//uint8_t TX1[]={66,33,33,33,33};
	uint8_t clear=0x38;
	SPI_Init();
	cleBit(PORTB,CE);
	clear|=(1<<MASK_RX_DR);
	NRF_WriteReg(CONFIG,&clear,1);//NRF_Init();
	//NRF_FlashRx();
	NRF_WriteReg(RX_ADDR_P0+0,pipe0,5);//NRF_SetPipeAddress(0,pipe0,5);
	NRF_WriteReg(TX_ADDR,TX0,5);//NRF_SetTransmitAddress(TX0,5);
	uint8_t size=1;
	NRF_WriteReg(RX_PW_P0+0,&size,1);
	NRF_ReadReg(EN_RXADDR,&size,1);
	size|=(1<<0);
	NRF_WriteReg(EN_RXADDR,&size,1);//NRF_OpenPipe(0,1);
	
	uint8_t value=0;
	NRF_ReadReg(CONFIG,&value,1);
	value|=(1<<PWR_UP);
	NRF_WriteReg(CONFIG,&value,1);
	_delay_ms(2);//NRF_PowerOn();
		
	value=0;
	NRF_ReadReg(CONFIG,&value,1);
	value|=(1<<PRIM_RX);
	NRF_WriteReg(CONFIG,&value,1);
	setBit(PORTB,CE);
	_delay_us(135);//NRF_ReceiveMode();
	TCCR0|=(1<<CS02)|(0<<CS01)|(1<<CS00);
 	DDRC=(1<<PC5);
 	PORTC^=(1<<PC5);
 	//_delay_ms(100);
 	//PORTC^=(1<<PC5);
	while (1) 
    {
		if((TIFR&(1<<TOV0))>0){
			TIFR|=1<<TOV0;
			pause++;
			if(pause>=152){
				NRF_FlashRx();
				//boot();
				//Enable IRQ on RX NRF_...!!!
				start();
				while(1){}
			}
		}
		switch(state_machine){
			case 0:
			if(NRF_DataAvailable()){
				pause=0;
				cleBit(PORTB,CSN);
				SPI_ReadWriteByte(R_RX_PAYLOAD);
				num_of_pages=SPI_ReadWriteByte(NOP);
				setBit(PORTB,CSN);//NRF_ReadPLD(&buf,1);
				
				uint8_t status=(1<<RX_DR);
				NRF_WriteReg(STATUS,&status,1);//NRF_StatusClear();
				state_machine=1; 
			}
				
			break;
			case 1:
					while(NRF_DataAvailable()){
						pause=0;
						cleBit(PORTB,CSN);
						SPI_ReadWriteByte(R_RX_PAYLOAD);
						//buf=SPI_ReadWriteByte(NOP);
						memory[counter]=SPI_ReadWriteByte(NOP);
						setBit(PORTB,CSN);//NRF_ReadPLD(&buf,1);
						
						uint8_t status=(1<<RX_DR);
						NRF_WriteReg(STATUS,&status,1);//NRF_StatusClear();
						//memory[counter]=buf;
						counter++;
						if(counter>=64){
							page++;
							boot_program_page((page-1)*SPM_PAGESIZE,memory);
							counter=0;
							if(page==num_of_pages){
								state_machine=2; 
							}
						}
					}
			break;
			case 2:
					NRF_ReadReg(CONFIG,&clear,1);
					clear&=~(1<<MASK_RX_DR);
					NRF_WriteReg(CONFIG,&clear,1);//NRF_EN_IRQ
					NRF_FlashRx();
					boot_rww_enable ();
					start();
					while(1){}
			break;
		}
    }
}

void boot_program_page (uint32_t page, uint8_t *buf)
{
	uint16_t i;
	uint8_t sreg;

	// Disable interrupts.

	sreg = SREG;
	cli();
	
	eeprom_busy_wait ();

	boot_page_erase (page);
	boot_spm_busy_wait ();      // Wait until the memory is erased.

	for (i=0; i<SPM_PAGESIZE; i+=2)
	{
		// Set up little-endian word.

		uint16_t w = *buf++;
		w += (*buf++) << 8;
		
		boot_page_fill (page + i, w);
	}

	boot_page_write (page);     // Store buffer in flash page.
	boot_spm_busy_wait();       // Wait until the memory is written.

	// Reenable RWW-section again. We need this if we want to jump back
	// to the application after bootloading.

	boot_rww_enable ();

	// Re-enable interrupts (if they were ever enabled).

	SREG = sreg;
}


