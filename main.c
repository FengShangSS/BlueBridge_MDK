#include "sys.h"

/*-------------------------V0.0.1-----------------------------------
DATE:2019.1.31		TIME:21:04		AUTHOR:FS
USAGE:
Smg.disbuff[]
led_control(uchar ledbit, uchar command)
relay_or_buzzer(uchar choice, uchar command)
-------------------------------------------------------------------*/

COUNT_t Count;
FLAG_t Flag;

void main()
{
	sysInit();
	while(1);
}

void timer0() interrupt 1
{
	display();
}

void timer1() interrupt 3
{
	Count.count0++;
	if(Count.count0 >= 2500)
	{
		Count.count0 = 0;
		Flag.flag0++;
		if(Flag.flag0&0x01 == 1)
		{
			led_control(0, ON);
			led_control(7, ON);
//			relay_or_buzzer(RELAY, ON);
//			relay_or_buzzer(BUZZ, OFF);
		}
		else
		{
			led_control(0, OFF);
			led_control(7, OFF);
//			relay_or_buzzer(RELAY, OFF);
//			relay_or_buzzer(BUZZ, ON);
		}
	}
}