/*
 * This be a header.  Argh!
 */
#include "LCD_Driver.h"
#include <msp430.h> 
#include "buttons/button.h"

#define LCD_SIZE 8

/*
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	char* string1 = "Message?";
	char* string2 = "Press123";
	char* string3 = "ECE382 is my favorite class!";
	char* message1 = "We found a witch.  Can we burn her?";
	char* message2 = "Insert cash or select payment type.";
	char* message3 = "All your base are belong to us.";

	char buttons[] = {BIT1, BIT2, BIT3};

	// Initialize.
	initSPI();
	initLCD();
	LCDclear();

	// Print first screen.
	writeString(string1);
	setCursorLine2();
	writeString(string2);

	// Configure push buttons.
	configureP1PinAsButton(BIT1|BIT2|BIT3);

	// Poll buttons
	char buttonPressed = pollP1Buttons(buttons, 3);
	LCDclear();

	switch (buttonPressed) {

	            case BIT1:

	            	scrollString(string3, message1, LCD_SIZE);
	                waitForP1ButtonRelease(BIT1);
	                break;

	            case BIT2:

	            	scrollString(string3, message2, LCD_SIZE);
	                waitForP1ButtonRelease(BIT2);
	                break;

	            case BIT3:

	            	scrollString(string3, message3, LCD_SIZE);
	                waitForP1ButtonRelease(BIT3);
	                break;
	        }


	return 0;
}
