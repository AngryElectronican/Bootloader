/*
   AVR_NRF.c

   Created: 15.09.2017 23:09:52
   Author : ANGRY_ELECTRONICAN
*/

#include <avr/io.h>
#include <avr/boot.h>
//#include "spi.h"
#define F_CPU 8000000UL
extern "C" {
#include "NRF.h"
};
/*uint8_t ALL_MEM[96]={0x12,0xC0,0x19,0xC0,0x18,0xC0,0x17,0xC0,0x16,0xC0,0x15,0xC0,0x14,0xC0,0x13,0xC0,0x12,0xC0,0x11,0xC0,0x10,0xC0,0x0F,0xC0,0x0E,0xC0,0x0D,0xC0,0x0C,0xC0,0x0B,0xC0,
  0x0A,0xC0,0x09,0xC0,0x08,0xC0,0x11,0x24,0x1F,0xBE,0xCF,0xE5,0xD4,0xE0,0xDE,0xBF,0xCD,0xBF,0x02,0xD0,0x13,0xC0,0xE4,0xCF,0x80,0xE2,0x87,0xBB,0x14,0xBA,0x85,0xBB,
  0x90,0xE2,0x88,0xB3,0x89,0x27,0x88,0xBB,0x2F,0xEF,0x30,0xE7,0x82,0xE0,0x21,0x50,0x30,0x40,0x80,0x40,0xE1,0xF7,0x00,0xC0,0x00,0x00,0xF3,0xCF,0xF8,0x94,0xFF,0xCF
    };*/
int main(void)
{
  Serial.begin(9600);
  uint8_t memory[64]={0};
  uint8_t needed_page=0,counter=0,page_counter=0;
  uint8_t temp=0;
  uint8_t buf = 0,buf1=0xF0;
  uint8_t PL[5]={0x31,0x31,0x31,0x31,0x31};
  uint8_t state=0;
  uint8_t pipe0[]={33,33,33,33,33};
  uint8_t pipe1[]={66,33,33,33,33};
  uint8_t TX[]={33,33,33,33,33};
  uint8_t TX1[]={66,33,33,33,33};
  NRF_Init();
  
  NRF_SetPipeAddress(0,pipe0,5);
  //NRF_SetPipeAddress(1,pipe1,5);
  DDRB|=1<<PB7;
  NRF_SetTransmitAddress(pipe0,5);
  //NRF_SetTransmitAddress(pipe0,5);
  NRF_OpenPipe(0,1);
  NRF_OpenPipe(1,1);
  NRF_FlashTx();
  NRF_FlashRx();
  NRF_PowerOn();
  //NRF_ReceiveMode();
 /*uint8_t needed_page=2;
        NRF_TransmitMode();
        NRF_WritePLD(&needed_page, 1);
        NRF_WriteData();
  for(int i=0;i<needed_page*64;i++){
        NRF_TransmitMode();
        NRF_WritePLD(&ALL_MEM[i], 1);
        NRF_WriteData();
        PORTB^=(1<<PB7); 
    }
     */  
//Serial.print("+");
     //Serial.print("D");
  while (1)
  {
    switch(state){
      case 0:
      if ( UCSR0A & (1<<RXC0)){
        needed_page=UDR0;
        //needed_page-=0x30;
        NRF_TransmitMode();
        NRF_WritePLD(&needed_page, 1);
        NRF_WriteData();
        //delay(10);
        Serial.print("+");
        state=1;
        }
      
      break;
      case 1:
      if(page_counter<needed_page){
      while ( UCSR0A & (1<<RXC0)){
        temp=UDR0;
        memory[counter]=temp;
        //Serial.write(temp);
        if(counter>=63){
          counter=0;
          page_counter++;
          for(int i=0;i<64;i++){
            NRF_TransmitMode();
            NRF_WritePLD(&memory[i], 1);
            NRF_WriteData();
            }
          Serial.print("+");
        }else{
        counter++;
        }
      }
      }else{
        NRF_FlashTx();
        NRF_FlashRx();
        state=0;
        counter=0;
        //Serial.print("FINISH\n");
        }
      break;
      
      }
    /*
    //NRF_ReadReg(STATUS,&data,1);
    //Serial.write(data);
    if(NRF_DataAvailable()){
      data=NRF_ReadPipe();
      NRF_ReadPLD(&buf,1); 
          Serial.print(data);
          Serial.print(" ");
          Serial.println(buf);
          PORTB^=(1<<PB7);
    }
        buf1++;
        NRF_TransmitMode();
        NRF_WritePLD(&buf1, 1);
        NRF_WriteData();
       // NRF_ReceiveMode();
    _delay_ms(100);*/
  }
}


