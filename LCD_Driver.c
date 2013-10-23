/*
 * LCD_Driver.c
 *
 *  Created on: Oct 23, 2013
 *      Author: C15Michael.Bentley
 */
#include "LCD_Driver.h"

//
void initSPI() {
	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 |= UCCKPH | UCMST | UCSYNC;

	UCB0CTL1 |= UCSEL1;
	UCB0STAT |= UCLISTEN;

	P1SEL |= BIT5 | BIT6 | BIT7;
	P1SEL2 |= BIT5 | BIT6 | BIT7;

	UCB0CTL1 &= UCSWRST;

}
void SS_Hi() {
	P1DIR |= BIT0;
	P1OUT |= BIT0;
}

void SS_Lo() {
	P1DIR |= BIT0;
	P1OUT &= BIT0;
}

// This delay is approx 40.5us for the native clock speed of 1.085MHz
void delayShort() {
	_delay_cycles(44);
}

// This delay is approx 1.65ms for the native clock speed of 1.085MHz
void delayLong() {
	_delay_cycles(1790);
}

// Writes a byte to the LCD
void LCD_Write_8(char byteToSend) {

	    unsigned char sendByte = byteToSend;

	    sendByte &= 0xF0;

	    sendByte = sendByte >> 4;               // rotate to the right 4 times

	    LCD_write_4(sendByte);

	    sendByte = byteToSend;

	    sendByte &= 0x0F;

	    LCD_write_4(sendByte);

}

void initLCD() {

}

