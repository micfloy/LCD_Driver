/*
 * LCD_Driver.h
 *
 *  Created on: Oct 23, 2013
 *      Author: C15Michael.Bentley
 */

#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

void initSPI();

void initLCD();

void LCDClear();

void setCursorLine1();

void setCursorLine2();

void writeChar(char singleChar);

void writeString(char*string);

void scrollString(char*string1, char*string2);


#endif /* LCD_DRIVER_H_ */
