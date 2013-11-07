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

// Sets slave select high.
void SS_Hi() {
	P1OUT |= BIT0;
}


// Sets slave select low.
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

// Configures the MSP430 to send information to the LCD.
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

//  This code writes a 4-bit piece of information to the LCD.
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

// Accepts a 4-bit command and sends it to the LCD.
void writeCommandNibble(char comNibble) {
	LCDCON &= ~RS_MASK
	;
	LCD_write_4(comNibble);
	delayLong();
}

// Accepts a byte-size command and sends it to the LCD.
void writeCommandByte(char comByte) {
	LCDCON &= ~RS_MASK
	;
	LCD_write_8(comByte);
	delayLong();
}

// Writes a byte-sized piece of Data to the LCD.
void writeDataByte(char dataByte) {
	LCDCON |= RS_MASK
	;
	LCD_write_8(dataByte);
	delayLong();
}

// Initializes the LCD to work with the MSP430.
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

// Clears the LCD and sets the cursor to the beginning of the first line.
void LCDclear() {
	writeCommandByte(1);

}

// Sets the cursor to the beginning of the first line without clearing the display.
void setCursorLine1() {
	writeCommandByte(0x02);
}

// Sets the cursor to line 2 without clearing the display.
void setCursorLine2() {
	writeCommandByte(0xC0);
}

// Writes a single character to the LCD.
void writeChar(char asciiChar) {
	writeDataByte(asciiChar);
}

// Writeds a string of characters to the LCD.
void writeString(char *string) {
	char *current = string;
	while (*current != 0) 				// Prints characters until the loop reaches the end of the string.
	{
		writeChar(*current);
		current++;
	}
}

// Helper function to the scroll string.  Returns an incremented current pointer.
char * printFromPosition(char * start, char * current, int screenSizeInChars) {
	if (*current == 0) {				// Resets the current pointer to the first place in the string.
		current = start;
	}
	char* dispChar = current;			// Temporary pointer.
	int i;
	for (i = 0; i < screenSizeInChars; i++) {
		writeDataByte(*dispChar);
		dispChar++;
		if (*dispChar == 0) {			// Resets temporary pointer to the first place in the string.
			dispChar = start;
		}
	}

	return ++current;

}

// Scrolls two strings across the bottom and top of the LCD display.
void scrollString(char *string1, char *string2, int screenSizeInChars) {
	char* current1 = string1;
	char* current2 = string2;
	int i;
	while (1) {							// Infinitely scrolls both strings.
		setCursorLine1();
		current1 = printFromPosition(string1, current1, screenSizeInChars);
		setCursorLine2();
		current2 = printFromPosition(string2, current2, screenSizeInChars);
		for(i = 0; i < 300; i++) {
			delayLong();
		}
	}
}

