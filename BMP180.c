#include "BMP180.h"

void BMP180_Init(void)
{
    bmp180._oss=1;
    short result=0;
    unsigned char addr=I2C_BMP180_StartCalAddr;
    for (unsigned char i=0; i<11; ++i)
    {
        I2C_StartCondition();
        I2C_Write_Byte(I2C_BMP180_RD);
        I2C_Write_Byte(addr);
        I2C_RepeatedStart();
        I2C_Write_Byte(I2C_BMP180_WR);
        result = (short)I2C_Read_Byte() << 8;
        I2C_ACK();
        result |= I2C_Read_Byte();
        I2C_NACK();
        I2C_StopCondition();
        
        switch (i)
        {
            case 0:
                {
                    bmp180._AC1 = result; break;
                }
            case 1:
                {
                    bmp180._AC2 = result; break;
                }
            case 2:
                {
                    bmp180._AC3 = result; break;
                }
            case 3:
                {
                    bmp180._AC4 = (unsigned short)result; break;
                }
            case 4:
                {
                    bmp180._AC5 = (unsigned short)result; break;
                }
            case 5:
                {
                    bmp180._AC6 = (unsigned short)result; break;
                }
            case 6:
                {
                    bmp180._B1 = result; break;
                }
            case 7:
                {
                    bmp180._B2 = result; break;
                }
            case 8:
                {
                    bmp180._MB = result; break;
                }
            case 9:
                {
                    bmp180._MC = result; break;
                }
            case 10:
                {
                    bmp180._MD = result; break;
                }
            default:
                break;
        }
        addr += 2;
    }
}

void BMP180_ReadData()
{
    I2C_StartCondition();
    I2C_Write_Byte(I2C_BMP180_RD);
    I2C_Write_Byte(0xF4);
    I2C_Write_Byte(0x2E);
    I2C_StopCondition();
    __delay_ms(10);
    I2C_StartCondition();
    I2C_Write_Byte(I2C_BMP180_RD);
    I2C_Write_Byte(0xF6);
    I2C_RepeatedStart();
    I2C_Write_Byte(I2C_BMP180_WR);
    bmp180.UT = I2C_Read_Byte() << 8;
    I2C_ACK();
    bmp180.UT |= I2C_Read_Byte();
    I2C_NACK();
    I2C_StopCondition();
    
    I2C_StartCondition();
    I2C_Write_Byte(I2C_BMP180_RD);
    I2C_Write_Byte(0xF4);
    I2C_Write_Byte(0x74);
    I2C_StopCondition();
    __delay_ms(10);
    I2C_StartCondition();
    I2C_Write_Byte(I2C_BMP180_RD);
    I2C_Write_Byte(0xF6);
    I2C_RepeatedStart();
    I2C_Write_Byte(I2C_BMP180_WR);
    bmp180.UP = (long)I2C_Read_Byte() << 16;
    I2C_ACK();
    bmp180.UP |= (long)I2C_Read_Byte() << 8;
    I2C_ACK();
    bmp180.UP |= I2C_Read_Byte();
    I2C_NACK();
    I2C_StopCondition();
    
    bmp180.UP >>= 8 - bmp180._oss;
    
    bmp180._x1=((bmp180.UT-bmp180._AC6)*bmp180._AC5) >> 15;
    bmp180._x2=((long)bmp180._MC<<11)/(bmp180._x1+bmp180._MD);
    bmp180._b5=bmp180._x1+bmp180._x2;
    bmp180.T = ((bmp180._b5+8)>>4);
    
    bmp180._b6 = bmp180._b5-4000;
    bmp180._x1 = (bmp180._b6*bmp180._b6)>>12;
    bmp180._x1 *= bmp180._B2;
    bmp180._x1 >>=11;
    
    bmp180._x2 = bmp180._AC2*bmp180._b6;
    bmp180._x2 >>= 11;
    
    bmp180._x3 = bmp180._x1+bmp180._x2;
    
    bmp180._b3 = (((((long)bmp180._AC1)*4+bmp180._x3)<<bmp180._oss)+2)>>2;
    bmp180._x1 = (bmp180._AC3*bmp180._b6)>>13;

    bmp180._x2 = (bmp180._B1*((bmp180._b6*bmp180._b6)>>12))>>16;

    bmp180._x3 = ((bmp180._x1+bmp180._x2)+2)>>2;
    
    bmp180._b4 = (bmp180._AC4*(unsigned long)(bmp180._x3+32768))>>15;

    bmp180._b7 = ((unsigned long)(bmp180.UP-bmp180._b3)*(50000>>bmp180._oss));
    
    bmp180.P = bmp180._b7<0x80000000 ? (bmp180._b7<<1)/bmp180._b4 : (bmp180._b7/bmp180._b4)<<1;
    
    bmp180._x1 = bmp180.P>>8;
    bmp180._x1 *= bmp180._x1;

    bmp180._x1 = (bmp180._x1*3038)>>16;

    bmp180._x2 = (bmp180.P*-7357)>>16;

    bmp180.P += (bmp180._x1+bmp180._x2+3791)>>4;
    bmp180.MMR = bmp180.P*0.0075;
}