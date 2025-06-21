#ifndef I2C_H
#define I2C_H
//--------------------------------------------------------------
#include <xc.h>
#define _XTAL_FREQ 20000000
void InitI2C(void);
void I2C_Idle(void);
void I2C_StartCondition(void);
void I2C_RepeatedStart(void);
void I2C_StopCondition(void);
void I2C_ACK(void);
void I2C_NACK(void);
unsigned char I2C_Write_Byte(unsigned char Byte);
unsigned char I2C_Read_Byte(void);
//--------------------------------------------------------------
#endif /* I2C_H */