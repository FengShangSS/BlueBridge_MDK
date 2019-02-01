#include "sys.h"
#include "keyscan.h"
#include "ds1302.h"

/*------------------------------------------------------------------
-------------------------V0.0.1-----------------------------------
DATE:2019.1.31		TIME:21:04		AUTHOR:FS
USAGE:
Smg.disbuff[]
led_control(uchar ledbit, uchar command)
relay_or_buzzer(uchar choice, uchar command)

-------------------------V0.0.2-----------------------------------
DATE:2019.2.01		TIME:19:31		AUTHOR:FS
USAGE:
Key
keyScan()
keyProcess(KEY_t *in, OUTPUT_t *out)

-------------------------V0.0.3-----------------------------------
DATE:2019.2.01		TIME:22:24		AUTHOR:FS
USAGE:
Time.set[7]
Time.read[7]
Ds_Control(TIME_t *time, uchar command)
-------------------------------------------------------------------*/

void freshDisbuff(void);
void timeUpdate(void);

COUNT_t Count;
FLAG_t Flag;
OUTPUT_t Output1;
TIME_t Time = {55,59,23, 1,2, 5, 19,\
				0,0,0, 0,0, 0, 0};

void main()
{
	sysInit();
	Ds_Control(&Time, WRITE);
	while(1)
	{
		if(Key.timeOk == 1)
		{
			Key.timeOk = 0;
			keyScan();
		}
		
		keyProcess(&Key, &Output1);
//		freshDisbuff();
		timeUpdate();
	}
}

void freshDisbuff(void)
{
	Smg.disbuff[5] = Output1.dat[0] / 100;
	Smg.disbuff[6] = Output1.dat[0] % 100 / 10;
	Smg.disbuff[7] = Output1.dat[0] % 10;
}

void timeUpdate(void)
{
	Ds_Control(&Time, READ);
	Smg.disbuff[0] = Time.read[2] / 10;
	Smg.disbuff[1] = Time.read[2] % 10;
	Smg.disbuff[2] = 10;
	Smg.disbuff[3] = Time.read[1] / 10;
	Smg.disbuff[4] = Time.read[1] % 10;
	Smg.disbuff[5] = 10;
	Smg.disbuff[6] = Time.read[0] / 10;
	Smg.disbuff[7] = Time.read[0] % 10;
}

void timer0() interrupt 1
{
	if(++Key.sacnCount == 5)
	{
		Key.sacnCount = 0;
		Key.timeOk = 1;
	}
	
	if(Key.mode == LONG)
	{
		led_control(7, ON);
		if(++Key.longPressCount >= 250)//0.5s
		{
			Key.longPressCount = 0;
			Key.longPressFlag = 1;
		}
	}
	else if(Key.mode == SHORT)
	{
		led_control(7, OFF);
		Key.longPressCount = 0;
		Key.longPressFlag = 0;
	}
}

void timer1() interrupt 3
{
	Count.count1++;
	if(Count.count1 >= 250)
	{
		Count.count1 = 0;
		Flag.flag1++;
		if(Flag.flag1&0x01 == 1)
		{
			led_control(0, ON);
//			relay_or_buzzer(RELAY, ON);
//			relay_or_buzzer(BUZZ, OFF);
		}
		else
		{
			led_control(0, OFF);
//			relay_or_buzzer(RELAY, OFF);
//			relay_or_buzzer(BUZZ, ON);
		}
	}
	display();
}