/*
 * This be a header.  Argh!
 */
#include "LCD_Driver.h"
#include <msp430.h> 
#include "buttons/button.h"

#define LCD_DISP_SIZE 8

/*
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	char* string1 = "ECE382 is my favorite class!";
	char* string2 = "Stop.  Hammertime!";

	initSPI();
	initLCD();
	LCDclear();

	scrollString(string1, string2, LCD_DISP_SIZE);

	return 0;
}
