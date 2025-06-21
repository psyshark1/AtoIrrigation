#include "RTC_1302.h"

void RTC_Init(void)
{
    TRISC &= ~(1 << 5);
    TRISC &= ~(1 << 6);
    TRISC &= ~(1 << 7);
    RTC_CLK = 0;
    RTC_RST = 0;
    RTC_RST = 1;
    RTC_send(0x8e); //write protect bit is 0, reset sec
    RTC_send(0);
    RTC_RST = 0;
    RTC_RST = 1;
    RTC_send(0x82); //reset minutes
    RTC_send(0);
    RTC_RST = 0;
    RTC_RST = 1;
    RTC_send(0x84); //24h standart, reset hours
    RTC_send(0);
    RTC_RST = 0;
    RTC_RST = 1;
    RTC_send(0x80);//start clock
    RTC_send(0);
    RTC_RST = 0;
}

void RTC_send(unsigned char value)
{
    //set the loop counter.
    for(unsigned char j=0;j<8;++j)          //continue to write 8bit
    {
        RTC_DAT = 0;                        //
        RTC_CLK = 0;         //pull low clk
        if(value&0x01)       //judge the send bit is 0 or 1.
        {
            RTC_DAT=1;       //is 1
        }
        value >>= 1;      //rotate right 1 bit.
        //RTC_DAT = (value >> j) & 1
        RTC_CLK = 1;         //pull high clk
    }
    RTC_CLK=0; 
}

unsigned char RTC_get(void)
{
    TRISC |= (1 << 7);
    unsigned char value=0;
    for(unsigned char j=0;j<8;++j)                  
    {                                         
        value |= (RTC_DAT << j);
        RTC_CLK = 1;//pull high clk
        RTC_CLK = 0;//pull low clk 
    }                                                              
    TRISC &= ~(1 << 7);//finished 1 byte,pull low clk  
    RTC_CLK=0;                          
    return value;
}