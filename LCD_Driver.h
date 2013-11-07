/*
 * LCD_Driver.h
 *
 *  Author: C2C Michael Bentley
 *  Created on: Oct 23, 2013
 *
 *  Description:  This library allows the user to print and scroll strings of characters to an LCD display.
 */

#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

void initSPI();

void initLCD();

void LCDclear();

void setCursorLine1();

void setCursorLine2();

void writeChar(char singleChar);

void writeString(char*string);

void scrollString(char*string1, char*string2, int screenSizeInChars);


#endif /* LCD_DRIVER_H_ */
