/*
 * This be a header.  Argh!
 */
#include "LCD_Driver.h"
#include <msp430.h> 

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    char* string= "ECE382 is my favorite class!";
    initSPI();
    initLCD();
    LCDclear();

    writeString(string);

	while(1) {

	}
}
