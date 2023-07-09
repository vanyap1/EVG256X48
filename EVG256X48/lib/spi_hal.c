/*
Copyright (c) 2021 Devilbinder (Binder Tronics) https://bindertronics.com/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */ 
#include "spi_hal.h"


volatile uint8_t spi_rx_data = 0;
volatile uint8_t spi_txrx_done = 0;

ISR(SPI0_STC_vect){
	
	if(SPSR0 & 0x40){
		spi_rx_data = SPDR0;
		spi_rx_data = 0;
		spi_txrx_done = 1;
	}else{
		spi_rx_data = SPDR0;
		spi_txrx_done = 1;
	}
}


void spi_master_init(void){
	DDRB |= (1 << DDB3) | (1 << DDB5); 
	DDRD |= (1 << DDD7);
	//DDRB  &= ~(1 << DDB3);
	
	//DDRB |= 1 << DDB0;
	//PORTB |= 1 << PORTB0;
	
	
	//Bit 7 – SPIE: SPI Interrupt Enable
	//Bit 6 – SPE: SPI Enable
	//Bit 5 – DORD: Data Order
	//Bit 4 – MSTR: Master/Slave Select
	//Bit 3 – CPOL: Clock Polarity
	//Bit 2 – CPHA: Clock Phase  (MO=1:_-_; MO=1:-_-; )
	//Bits 1, 0 – SPR1, SPR0: SPI Clock Rate Select 1 and 0
	//00 - fosc/4
	//01 - fosc/16 
	//10 - fosc/64
	//11 - fosc/128
	SPCR0 = (1 << SPIE) | (1 << SPE) | (1 << MSTR)  | (0 << CPOL) | (0 << CPHA) | (0b00 << SPR0);
	SPSR0 = (1 << SPI2X);
}



void spi_send(uint8_t *tx,uint8_t *rx){
	spi_txrx_done = 0;
	SPDR0 = *tx;
	while(spi_txrx_done == 0);
	*rx = spi_rx_data;
}

void spi_tranfer(uint8_t *tx,uint8_t *rx,uint16_t len){
	for(uint16_t i=0; i < len;i++){
		spi_send(&tx[i],&rx[i]);
	}
}

void spi_tx_en(bool value){
	if (value) {
		PORTE &= ~(1 << PORTE2);
	}else{
		PORTE |= 1 << PORTE2;
	}	
}

void vfd_tx_en(bool value){
	if (value) {
		PORTD &= ~(1 << PORTD7);
		}else{
		PORTD |= 1 << PORTD7;
	}
}

void W5500_Select(void){
	spi_tx_en(true);
}

void W5500_Unselect(void){
	spi_tx_en(false);
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len){
	for(uint16_t i=0; i < len;i++){
		buff[i] = W5500_ReadByte();
	}
}
void W5500_WriteBuff(uint8_t* buff, uint16_t len){
	for(uint16_t i=0; i < len;i++){
		W5500_Write_byte(buff[i]);
	}
}
uint8_t W5500_ReadByte(void){
	spi_txrx_done = 0;
	SPDR0 = 0;
	while(spi_txrx_done == 0);
	return spi_rx_data;	
}
void W5500_Write_byte(uint8_t byte){
	spi_txrx_done = 0;
	SPDR0 = byte;
	while(spi_txrx_done == 0);
}





 
