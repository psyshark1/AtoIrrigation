#include "main.h"

inline void CheckButtons()
{
  states = (RA1) ? states | 1 : states & ~((unsigned short)0b0000000000000001);
  //states = (RA2) ? states | (1 << 1) : states & ~(1 << 1);
}

inline void SetLight()
{
    states = (!((PORTD >> 4) & 1)) ? states | (1 << 13) : states & ~((unsigned short)1 << 13);
    __delay_ms(3000);
    states = (!((PORTD >> 4) & 1)) ? states | (1 << 13) : states & ~((unsigned short)1 << 13);
}

inline void SetSoil()
{
    states = (!((PORTD >> 5) & 1)) ? states | (1 << 10) : states & ~((unsigned short)1 << 10);
    __delay_ms(2000);
    states = (!((PORTD >> 5) & 1)) ? states | (1 << 10) : states & ~((unsigned short)1 << 10);
}

inline void SetWater()
{
    states = (!((PORTD >> 3) & 1)) ? states | (1 << 12) : states & ~((unsigned short)1 << 12);
    __delay_ms(250);
    states = (!((PORTD >> 3) & 1)) ? states | (1 << 12) : states & ~((unsigned short)1 << 12);
}

void ConvertTime(unsigned char clock)
{
    RTC_RST = 1;
    RTC_send(clock);
    tmp = RTC_get();
    RTC_RST = 0;
    switch(clock)
    {
        case 0x81://sec
        {
            switch((tmp & 0b01110000) >> 4)
            {
                case 0:
                {
                    sec = tmp & 0b00001111;break;
                }
                case 1:
                {
                    sec = 10 + (tmp & 0b00001111);break;
                }
                case 2:
                {
                    sec = 20 + (tmp & 0b00001111);break;
                }
                case 3:
                {
                    sec = 30 + (tmp & 0b00001111);break;
                }
                case 4:
                {
                    sec = 40 + (tmp & 0b00001111);break;
                }
                case 5:
                {
                    sec = 50 + (tmp & 0b00001111);break;
                }
            }
            if (!((states >> 9) & 1) && ((states >> 14) & 1))// 20 sec wait
            {
                if (!((states >> 8) & 1))
                {
                    RTC_RST = 1;
                    RTC_send(0xC0);//RTC1302 RAM write
                    if((sec + pump_check_t) >= 60)
                    {
                        RTC_send(pump_check_t - (60 - sec));//calc control sec start pump check
                    }
                    else
                    {
                        RTC_send(pump_check_t + sec);
                    }
                    RTC_RST = 0;
                    states |= (1 << 8);
                }
                RTC_RST = 1;
                RTC_send(0xC1);
                tmp = RTC_get();
                RTC_RST = 0;
                if (tmp == sec) {states |= (1 << 9);} 
            }
            break;
        }
        case 0x83://min
        {
            switch((tmp & 0b01110000) >> 4)
            {
                case 0:
                {
                    min = tmp & 0b00001111;break;
                }
                case 1:
                {
                    min = 10 + (tmp & 0b00001111);break;
                }
                case 2:
                {
                    min = 20 + (tmp & 0b00001111);break;
                }
                case 3:
                {
                    min = 30 + (tmp & 0b00001111);break;
                }
                case 4:
                {
                    min = 40 + (tmp & 0b00001111);break;
                }
                case 5:
                {
                    min = 50 + (tmp & 0b00001111);break;
                }
            }
            if (((states >> 9) & 1))// 20 sec wait OK
            {
                if (!((states >> 7) & 1))
                {
                    RTC_RST = 1;
                    RTC_send(0xC2);//RTC1302 RAM write
                    if((min + irrigation_t) >= 60)
                    {
                        RTC_send(irrigation_t - (60 - min));//calc irrigation time disable pump
                    }
                    else
                    {
                        RTC_send(irrigation_t + min);
                    }
                    RTC_RST = 0;
                    states |= (1 << 7);
                }
                RTC_RST = 1;
                RTC_send(0xC3);
                tmp = RTC_get();
                RTC_RST = 0;
                if (tmp == min) {states |= (1 << 6);}
            }
            break;
        }
        case 0x85://hrs
        {
            switch((tmp & 0b00110000) >> 4)
            {
                case 0:
                {
                    hrs = tmp & 0b00001111;break;
                }
                case 1:
                {
                    hrs = 10 + (tmp & 0b00001111);break;
                }
                case 2:
                {
                    hrs = 20 + (tmp & 0b00001111);break;
                }
            }
            
            if (((states >> 15) & 1))
            {
                if (!((states >> 5) & 1))
                {
                    RTC_RST = 1;
                    RTC_send(0xC4);//RTC1302 RAM write
                    if((hrs + irrigation_t) >= 24)
                    {
                        RTC_send(irrigation_t - (24 - hrs));//calc cycle irrigation time
                    }
                    else
                    {
                        RTC_send(irrigation_t + hrs);
                    }
                    RTC_RST = 0;
                    states |= (1 << 5);
                }
                RTC_RST = 1;
                RTC_send(0xC5);
                tmp = RTC_get();
                RTC_RST = 0;
                if (tmp == hrs)
                {
                    states &= ~(1 << 15);
                    LCD_Clear_Row(4);
                    LCD_Set_Cursor(4,1);
                    LCD_Write_String("OK");
                }
            }
            break;
        }
    }
}

inline void PrintContrtolPanel()
{
    LCD_Set_Cursor(1,1);
    LCD_Write_String("Irrigation Control");
    LCD_Set_Cursor(2,1);
    LCD_Write_String("Temp:");
    LCD_Set_Cursor(2,10);
    LCD_Write_String("Press:");
    LCD_Set_Cursor(3,1);
    LCD_Write_String("Status Pump:");
}

inline void PrintError()
{
    LCD_Set_Cursor(3,13);
    LCD_Write_String("ERROR!");
    LCD_Set_Cursor(4,1);
    LCD_Write_String("Press OK to reset");
}

inline void ReadTemPress()
{
    BMP180_ReadData();
    LCD_Set_Cursor(2,6);
    snprintf(dat, sizeof(dat), "%ld", (bmp180.T / 10));
    LCD_Write_String(dat);
    LCD_Set_Cursor(2,16);
    snprintf(dat, sizeof(dat), "%d", bmp180.MMR);
    LCD_Write_String(dat);
}

void main(void)
{   
    TRISA = 0b00111110;
    PORTA = 0b00000000;
    
    TRISD = 0b00011100;//DI light sensor, soil sensor, pump watersensor
    PORTD = 0b00000000;
    
    WDTCON = 0b00010001;//watchdog 1:8192 prescaler
    
    InitI2C();// Initialize i2c
    LCD_Init(0x4E);
    RTC_Init();
    BMP180_Init();
    
    PrintContrtolPanel();
    LCD_Set_Cursor(3,13);
    LCD_Write_String("Ready");
    LCD_Set_Cursor(4,1);
    LCD_Write_String("OK");
    //__delay_ms(2000);
    //LCD_noBacklight();
    //LCD_Clear();
    //ReadTime();
    //
    ReadTemPress();
    
    while(1)
    {
        __asm("clrwdt");
        if(!((states >> 14) & 1))
        {
            PORTD &= ~(1 << 2);// pump relay disable
            if(!((states >> 11) & 1) && ((bmp180.T / 10) > 10) && !((states >> 15) & 1))
            {
                SetLight();
                SetSoil();
                if(((states >> 13) & 1) && ((states >> 10) & 1))
                {
                    states |= (1 << 14);
                }
            }
            else if (((states >> 11) & 1))
            {
                LCD_noBacklight();
                __delay_ms(2000);
                LCD_Backlight();
                __delay_ms(2000);
            }
            
            if ((states & 1))
            {
                states = 0b0100000000000000;
            }
        }
        else
        {
            if(((PORTD >> 2) & 1))
            {
                if(!((states >> 12) & 1) && !((states >> 11) & 1))//no error & no water
                {
                    if (((states >> 9) & 1))// 20 sec wait
                    {
                        SetWater();
                        if(!((states >> 12) & 1))//no water
                        {
                            PORTD &= ~(1 << 2);// pump relay disable
                            states |= (1 << 11);//set checkpump
                            states &= 0b0011110000100000;//resets
                            PrintError();
                        }
                    }
                }
                else if(((states >> 12) & 1) && !((states >> 11) & 1))//no error & water
                {
                    SetWater();
                    if(!((states >> 12) & 1))//no water
                    {
                        PORTD &= ~(1 << 2);// pump relay disable
                        states |= (1 << 11);//set checkpump
                        states &= 0b0011110000100000;//resets
                        PrintError();
                    }
                }
                
                SetSoil();
                
                if (!((states >> 10) & 1) || ((states >> 6) & 1))//wet soil sensor or irrigation time is out
                {
                    PORTD &= ~(1 << 2);
                    states = 0b1000000000000000;//set time cycle and resets
                    LCD_Set_Cursor(3,13);
                    LCD_Write_String("Ready");
                    LCD_Clear_Row(4);
                    LCD_Set_Cursor(4,1);
                    LCD_Write_String("Waiting");
                }
            }
            else if(!((states >> 11) & 1))//no checkpump
            {
                PORTD |= (1 << 2);// pump relay enable
                LCD_Set_Cursor(3,13);
                LCD_Write_String("Working");
                LCD_Clear_Row(4);
            }
            else
            {
                states &= ~((unsigned short)1 << 14);
                LCD_Set_Cursor(3,13);
                PrintError();
            }
        }
        
        ConvertTime(0x81);
        if (!sec)
        {
          ConvertTime(0x83);
          ReadTemPress();
          if (!min)
          {
             ConvertTime(0x85); 
          }
        }
        
        CheckButtons();
    }
    return;
}
