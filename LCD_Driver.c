/*
 * LCD_Driver.c
 *
 *  Created on: Oct 23, 2013
 *      Author: C15Michael.Bentley
 */
#include "LCD_Driver.h"
#include <msp430.h>

#define RS_MASK 0x40;

char LCDCON;

void SS_Hi();
void SS_Lo();

//
void initSPI() {
	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 |= UCCKPH | UCMST | UCSYNC | UCMSB;

	UCB0CTL1 |= UCSSEL1;

	P1SEL |= BIT5 | BIT6 | BIT7;
	P1SEL2 |= BIT5 | BIT6 | BIT7;

	UCB0CTL1 &= ~UCSWRST;

	P1DIR |= BIT0;

	SS_Hi();

}

void SS_Hi() {
	P1OUT |= BIT0;
}

void SS_Lo() {
	P1OUT &= ~BIT0;
}

// This delay is approx 40.5us for the native clock speed of 1.085MHz
void delayShort() {
	_delay_cycles(44);
}

// This delay is approx 1.65ms for the native clock speed of 1.085MHz
void delayLong() {
	_delay_cycles(1790);
}

void SPIsend(char byteToSend) {
	volatile char readByte;

	SS_Lo();

	UCB0TXBUF = byteToSend;

	while (!(UCB0RXIFG & IFG2)) {
		// wait until you've received a byte
	}

	readByte = UCB0RXBUF;

	SS_Hi();
}

//
void LCD_write_4(char nibbleToSend) {
	unsigned char sendNibble = nibbleToSend;
	sendNibble &= 0x0F; 					// Clear the top of the byte
	sendNibble |= LCDCON;

	sendNibble &= 0x7F; 					// Sets E to low
	SPIsend(sendNibble);
	delayShort();

	sendNibble |= 0x80; 					// Sets E to high
	SPIsend(sendNibble);
	delayShort();

	sendNibble &= 0x7F; 					// Sets E to low
	SPIsend(sendNibble);
	delayShort();
}

// Writes a byte to the LCD
void LCD_write_8(char byteToSend) {

	unsigned char sendByte = byteToSend;

	sendByte &= 0xF0;						// Clear the bottom of the byte

	sendByte = sendByte >> 4;               // rotate to the right 4 times

	LCD_write_4(sendByte);

	sendByte = byteToSend;

	sendByte &= 0x0F;						// Clear the top of the byte

	LCD_write_4(sendByte);

}

void writeCommandNibble(char comNibble) {
	LCDCON &= ~RS_MASK
	;
	LCD_write_4(comNibble);
	delayLong();
}

void writeCommandByte(char comByte) {
	LCDCON &= ~RS_MASK
	;
	LCD_write_8(comByte);
	delayLong();
}

void writeDataByte(char dataByte) {
	LCDCON |= RS_MASK
	;
	LCD_write_8(dataByte);
	delayLong();
}

void initLCD() {
	writeCommandNibble(0x03);

	writeCommandNibble(0x03);

	writeCommandNibble(0x03);

	writeCommandNibble(0x02);

	writeCommandByte(0x28);

	writeCommandByte(0x0C);

	writeCommandByte(0x01);

	writeCommandByte(0x06);

	writeCommandByte(0x01);

	writeCommandByte(0x02);

	SPIsend(0);
	delayShort();
}

void LCDclear() {
	writeCommandByte(1);

}

void setCursorLine1() {
	writeCommandByte(0x02);
}

void setCursorLine2() {
	writeCommandByte(0xC0);
}

void writeChar(char asciiChar) {
	writeDataByte(asciiChar);
}

void writeString(char *string) {
	while (string != 0) {
		writeChar(*string);
		string++;
	}
}

char * printFromPosition(char * start, char * current, int screenSizeInChars) {
	if (*current == 0) {
		current = start;
	}
	char* dispChar = current;
	int i;
	for (i = 0; i < screenSizeInChars; i++) {
		writeDataByte(*dispChar);
		dispChar++;
		if (*dispChar == 0) {
			dispChar = start;
		}
	}

	return ++current;

}

void scrollString(char *string1, char *string2, int screenSizeInChars) {
	char* current1 = string1;
	char* current2 = string2;
	int i;
	while (1) {
		setCursorLine1();
		current1 = printFromPosition(string1, current1, screenSizeInChars);
		setCursorLine2();
		current2 = printFromPosition(string2, current2, screenSizeInChars);
		for(i = 0; i < 500; i++) {
			delayLong();
		}
	}
}

