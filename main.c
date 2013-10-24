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
	
    initSPI();
    initLCD();
    LCDclear();

	while(1) {

	}
}
