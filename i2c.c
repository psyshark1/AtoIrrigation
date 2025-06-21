#include "i2c.h"
//------------------------------------------------
void InitI2C(void)
{
  TRISC=0b00011000;//RC3 and RC4 Input (SDC and SDA)
  SSPADD=0x31;//49
  SSPSTAT = 0b10000000; // Slew Rate control is disabled:
  // SMP=1 (Slew rate control disabled for standard speed mode (100 kHz and 1 MHz))
  // CKE=0(Disable SMBus specific inputs)
  SSPCON = 0b00111000; // Select and enable I2C in master mode:
  // SSPM3:SSPM0 = 1000; (I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
  // SSPEN=1 (Enables the serial port and configures the SDA and SCL pins as the serial port pins)
  // CKP=1 Release clock
}
//------------------------------------------------
inline void I2C_Idle(void)
{
    while((SSPCON2 & 0x1F)||(SSPSTAT & 0x04)){}
}
//------------------------------------------------
void I2C_StartCondition(void)
{
  I2C_Idle();
  SEN=1;
}

void I2C_RepeatedStart()
{
    I2C_Idle();
    RSEN = 1;
}
//------------------------------------------------
void I2C_StopCondition(void)
{
  I2C_Idle();
  PEN = 1;
}

void I2C_ACK(void)
{
	ACKDT = 0;			// 0 -> ACK
	I2C_Idle();
    ACKEN = 1;			// Send ACK
}

void I2C_NACK(void)
{
	ACKDT = 1;			// 1 -> NACK
    I2C_Idle();
	ACKEN = 1;			// Send NACK
}
//------------------------------------------------
unsigned char I2C_Write_Byte(unsigned char Byte)
{
  I2C_Idle();
  SSPBUF = Byte;
  while(!SSPIF){}
  SSPIF=0;
  return ACKSTAT;
}
unsigned char I2C_Read_Byte(void)
{
    I2C_Idle();
    RCEN = 1;		  // Enable & Start Reception
	while(!SSPIF){}	  // Wait Until Completion
	SSPIF = 0;		  // Clear The Interrupt Flag Bit
    I2C_Idle();
    return SSPBUF;	  // Return The Received Byt
}
