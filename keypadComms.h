#ifndef __KEYPADCOMMS_H__
#define __KEYPADCOMMS_H__

void keypad_transmit(void);

void keypad_receive(void);

void transmitAndReceive(void);

int buttonPressed(void);

short int determineRow(void);

char scanKeypad(void);

char forceGetKeypadValue(void);


#endif
