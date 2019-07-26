/*
 * registersNRF.h
 *
 * Created: 19.09.2017 22:46:05
 *  Author: ANGRY_ELECTRONICAN
 */ 


#ifndef REGNRF_H_
#define REGNRF_H_

//**********Registers**********//
enum{
    CONFIG  = 0x00,
    EN_AA,
    EN_RXADDR,
    SETUP_AW,
    SETUP_RETR,
    RF_CH,
    RF_SETUP,
    STATUS,
    OBSERVE_TX,
    CD,
    RX_ADDR_P0,
    RX_ADDR_P1,
    RX_ADDR_P2,
    RX_ADDR_P3,
    RX_ADDR_P4,
    RX_ADDR_P5,
    TX_ADDR,
    RX_PW_P0,
    RX_PW_P1,
    RX_PW_P2,
    RX_PW_P3,
    RX_PW_P4,
    RX_PW_P5,
    FIFO_STATUS_REG
	};
	//CONFIG
	//Reserved 7
	#define MASK_RX_DR	6
	#define MASK_TX_DS	5
	#define MASK_MAX_RT 4
	#define EN_CRC		3
	#define CRCO		2
	#define PWR_UP		1
	#define PRIM_RX		0
	
	//EN_AA
	//Reserved 7
	//Reserved 6
	#define ENAA_P5 5
	#define ENAA_P4 4
	#define ENAA_P3 3
	#define ENAA_P2 2
	#define ENAA_P1 1
	#define ENAA_P0 0
	
	//EN_RXADDR
	//Reserved 7
	//Reserved 6
	#define ERX_P5 5
	#define ERX_P4 4
	#define ERX_P3 3
	#define ERX_P2 2
	#define ERX_P1 1
	#define ERX_P0 0
	
	//STATUS
	//Reserved
	#define RX_DR		6
	#define TX_DS		5
	#define MAX_RT		4
	#define RX_P_NO_2	3
	#define RX_P_NO_1	2
	#define RX_P_NO_0	1
	#define TX_FULL		0
	
	
	//**********COMANDS**********//
	#define R_REGISTER			0x00
	#define W_REGISTER			0x20
	#define R_RX_PAYLOAD		0x61
	#define W_TX_PAYLOAD		0xA0
	#define FLUSH_TX			0xE1
	#define FLUSH_RX			0xE2
	#define REUSE_TX_PL			0xE3
	#define NOP					0xFF
	



#endif /* REGNRF_H_ */