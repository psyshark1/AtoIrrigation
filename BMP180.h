#ifndef BMP180_H
#define	BMP180_H

//#include <xc.h>
#include "i2c.h"

#define I2C_BMP180_RD 0xEE //0x77 with 0 bit
#define I2C_BMP180_WR 0xEF //0x77 with 1 bit
#define I2C_BMP180_StartCalAddr 0xAA

struct calibrate
{
    short _AC1, _AC2, _AC3, _B1, _B2, _MB, _MC, _MD;
    unsigned short _AC4, _AC5, _AC6, MMR;
    unsigned char _oss;
    long _x1, _x2, _x3, _b3, _b5, _b6, UP, UT, T, P;
    unsigned long _b4, _b7;
    //float MMR, TT;
} bmp180;

void BMP180_Init(void);
void BMP180_ReadData(void);
#endif	// BMP180_H

