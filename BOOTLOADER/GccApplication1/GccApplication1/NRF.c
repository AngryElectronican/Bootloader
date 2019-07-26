/*
 * NRF.c
 *
 * Created: 17.09.2017 22:29:59
 *  Author: ANGRY_ELECTRONICAN
 */ 
#include "NRF.h"
#include "spi.h"
 /*struct PINS {
	uint8_t PORT;
	uint8_t CE;
	uint8_t CSN;
	}NRF;
	struct PINS NRF={0,0,0};*/
void NRF_ReadReg(uint8_t addr,uint8_t *buffer,uint8_t size){
	cleBit(PORTB,CSN);
	SPI_ReadWriteByte(addr);
	for(int i=0;i<size;i++){
	  buffer[i]=SPI_ReadWriteByte(NOP);
	}
	setBit(PORTB,CSN);
}
void NRF_WriteReg(uint8_t addr,uint8_t *value,uint8_t num){
	cleBit(PORTB,CSN);
	SPI_ReadWriteByte(addr|0x20);
	for(int i=0;i<num;i++){
		SPI_ReadWriteByte(value[i]);
	}
	setBit(PORTB,CSN);
}
void NRF_FlashTx(){
  cleBit(PORTB,CSN);
  SPI_ReadWriteByte(0xE1);
  setBit(PORTB,CSN);
}
void NRF_FlashRx(){
  cleBit(PORTB,CSN);
  SPI_ReadWriteByte(0xE2);
  setBit(PORTB,CSN);
}

void NRF_StatusClear(){
  uint8_t status=(1<<RX_DR);
  NRF_WriteReg(STATUS,&status,1);
  }
void NRF_Init(){
	uint8_t clear=0x38;
	SPI_Init();
	cleBit(PORTB,CE);
	NRF_WriteReg(CONFIG,&clear,1);
}
void NRF_PowerOff(void){
	uint8_t value=0;
	NRF_ReadReg(CONFIG,&value,1);
	value&=~(1<<PWR_UP);
	NRF_WriteReg(CONFIG,&value,1);
	_delay_ms(2);
}
void NRF_PowerOn(void){
	uint8_t value=0;
	NRF_ReadReg(CONFIG,&value,1);
	value|=(1<<PWR_UP);
	NRF_WriteReg(CONFIG,&value,1);
	_delay_ms(2);
}
void NRF_ReceiveMode(void){
	uint8_t value=0;
	NRF_ReadReg(CONFIG,&value,1);
	value|=(1<<PRIM_RX);
	NRF_WriteReg(CONFIG,&value,1);
	setBit(PORTB,CE);
	_delay_us(135);
}
void NRF_TransmitMode(){
	uint8_t value=0;
	NRF_ReadReg(CONFIG,&value,1);
	value&=~(1<<PRIM_RX);
	NRF_WriteReg(CONFIG,&value,1);
	cleBit(PORTB,CE);
	_delay_us(135);
}
void NRF_WritePLD(uint8_t *buf,uint8_t size){
	cleBit(PORTB,CSN);
	SPI_ReadWriteByte(W_TX_PAYLOAD);
	for(int i=0; i<size;i++){
	  SPI_ReadWriteByte(buf[i]);
	}
	setBit(PORTB,CSN);
}
void NRF_ReadPLD(uint8_t *buf,uint8_t size){
  uint8_t status=0;
  cleBit(PORTB,CSN);
  SPI_ReadWriteByte(R_RX_PAYLOAD);
	for(int i=0;i<size;i++){
		buf[i]=SPI_ReadWriteByte(NOP);
	}
	setBit(PORTB,CSN);
}
bool NRF_AcknowledgmentReceived(){
	uint8_t status=0;
	NRF_ReadReg(STATUS,&status,1);
	return (((status)&(1<<TX_DS))>0);
}
bool NRF_DataAvailable(){
  uint8_t status=0;
  NRF_ReadReg(STATUS,&status,1);    
  return (((status>>1)&0x07)<6)||(status&(1<<RX_DR)>0);
}
void NRF_ResetRetransmitCounter(){
	uint8_t status=(1<<MAX_RT);
	NRF_WriteReg(STATUS,&status,1);
}
void NRF_WriteData(){
  uint8_t status=0;
  setBit(PORTB,CE);
  _delay_us(15);
  while(!NRF_AcknowledgmentReceived())
  {
    NRF_ResetRetransmitCounter();
  }
  NRF_ReadReg(STATUS,&status,1);
  status=(1<<TX_DS);
  NRF_WriteReg(STATUS,&status,1);
  cleBit(PORTB,CE);
}
void NRF_OpenPipe(uint8_t pipe,uint8_t width){
  uint8_t size=width;
  NRF_WriteReg(RX_PW_P0+pipe,&size,1);
  NRF_ReadReg(EN_RXADDR,&size,1);
  size|=(1<<pipe);
  NRF_WriteReg(EN_RXADDR,&size,1);
}
void NRF_SetPipeAddress(uint8_t pipe,uint8_t *addr,uint8_t num){
	NRF_WriteReg(RX_ADDR_P0+pipe,addr,num);
}
void NRF_EnAutoAck(uint8_t pipe){
	
}
void NRF_SetTransmitAddress(uint8_t *addr,uint8_t num){
	NRF_WriteReg(TX_ADDR,addr,num);
}
uint8_t NRF_ReadPipe(){
  uint8_t pipe=0;
  NRF_ReadReg(STATUS,&pipe,1);
  pipe=(pipe>>1)&0x07;
  return pipe;
}
