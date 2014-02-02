#ifndef __LCDINIT_H__
#define __LCDINIT_H__

void lcd_transmit(void);

void checkBusyFlag(void);

char asciiShift(char c);

void selectLine(int line);

void sendASCII(char *string);

void clearLine(int line);

void sendChar(char character);

void lcd_init(void);

#endif
