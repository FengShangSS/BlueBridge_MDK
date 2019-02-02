#include "menu.h"
#include "keyscan.h"
#include "ds1302.h"
#include "onewire.h"

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
keyProcess(KEY_t *in, MENU_t *menu)

-------------------------V0.0.3-----------------------------------
DATE:2019.2.01		TIME:22:24		AUTHOR:FS
USAGE:
Time.set[7]
Time.read[7]
Ds_Control(TIME_t *time, uchar command)

-------------------------V0.0.4-----------------------------------
DATE:2019.2.02		TIME:15:37		AUTHOR:FS
USAGE:
Temperature.integer
Temperature.fraction
temp_read(TEMP_t *temperature)

Menu.mode
Menu.sub[]
menuUpdate(MENU_t *menu)
freshDisbuff(SMG_t *smg)

-------------------------------------------------------------------*/


COUNT_t Count;
FLAG_t Flag;

void main()
{
	sysInit();
	Ds_Control(&Time, WRITE);
	while(1)
	{
		if(Temperature.timeOk == 1)
		{
			Temperature.timeOk = 0;
			temp_read(&Temperature);
		}
		
		if(Key.timeOk == 1)
		{
			Key.timeOk = 0;
			keyScan();
			keyProcess(&Key, &Menu);
		}
		
		if(Time.runFlag)
			Ds_Control(&Time, READ);

		menuUpdate(&Menu);
	}
}

void timer0() interrupt 1
{
	if(++Temperature.scanCount == 100)//0.2s
	{
		Temperature.scanCount = 0;
		Temperature.timeOk = 1;
	}
	
	if(++Key.sacnCount == 5)//0.01s
	{
		Key.sacnCount = 0;
		Key.timeOk = 1;
	}
	
	if(Key.mode == LONG)
	{
		led_control(7, ON);
		if(++Key.longPressCount >= 125)//0.25s
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
	if(Count.count1 >= 250)//0.5s
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