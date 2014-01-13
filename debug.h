#ifndef __DEBUG_H__
#define __DEBUG_H__

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define BUF_MAX 100

int read_usb_serial_none_blocking(char *buf, int length);

int write_usb_serial_blocking(char *buf, int length);

int printToTerminal(char *buf);

int printfToTerminal(char *buf, ...);

void dumpRegs();

void setColor();

void debugAssert(char *message, char *file, int line);

void debugWarn(char *message, char *file, int line);

void debugThrow(char *message, char *file, int line);

void debug_init(void);

//Main assert macro!

#define ASSERT(X, Y)  { if (X) {} else { debugAssert(Y, __FILE__, __LINE__); } } 
#define WARN(X, Y)    { if (X) {} else { debugWarn(Y, __FILE__, __LINE__);   } }
#define THROW(X)          debugThrow(X, __FILE__, __LINE__);

#endif
