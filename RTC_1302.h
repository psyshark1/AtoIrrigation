#ifndef RTC_H
#define	RTC_H

#include "BMP180.h"

#define RTC_DAT RC7     //1302 data           
#define RTC_CLK RC6     //1302 clock        
#define RTC_RST RC5     //1302 enable bit 

void RTC_Init(void);
void RTC_send(unsigned char value);
unsigned char RTC_get(void);

#endif	/* RTC_H */

