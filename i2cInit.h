#ifndef __I2CINIT_H__
#define __I2CINIT_H__

I2C_M_SETUP_Type transferCfg;
PINSEL_CFG_Type PinCfg;

void pollAddresses(void);

int i2c_init(void);


#endif
