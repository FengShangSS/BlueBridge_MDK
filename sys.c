#include "sys.h"

uchar tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,\
				0xc0-0x80,0xf9-0x80,0xa4-0x80,0xb0-0x80,0x99-0x80,\
				0x92-0x80,0x82-0x80,0xf8-0x80,0x80-0x80,0x90-0x80,\
				0xff,0xbf,0xc6,0x8E,0xC1};

SMG_t Smg = {0, 11,11,11,11,11,11,11,11};
LED_t Led = {0xFF, 0x00};
LED_t RelayOrBuzz = {0x00, 0x00};

void sysInit(void)
{
	IOinit();
	Timer0Init();
	Timer1Init();
	EA = 1;
}

void IOinit(void)
{
	P2 &= NONE;
	P2 |= LEDS;P0 = 0xFF;P2 &= NONE; //LED   VCC
	P2 |= OTHS;P0 = 0x00;P2 &= NONE; //RELAY & BUZZER
	P2 |= BITS;P0 = 0xFF;P2 &= NONE; //BIT   
	P2 |= SEGS;P0 = 0XFF;P2 &= NONE; //SEG   VCC
}

void Timer0Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x9A;		//设置定时初值
	TH0 = 0xA9;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;		//使能定时器0中断
}


void Timer1Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x9A;		//设置定时初值
	TH1 = 0xA9;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;		//使能定时器1中断
}

void display()
{
	P2 |= SEGS;
	P0 = 0xFF;
	P2 &= NONE;
	
	P2 |= BITS;
	P0 = 0x01 << Smg.disbit;
	P2 &= NONE;
	
	P2 |= SEGS;
	P0 = tab[Smg.disbuff[Smg.disbit]];
	P2 &= NONE;
	
	if(++Smg.disbit == 8)
		Smg.disbit = 0;
}

void led_control(uchar ledbit, uchar command)
{
	if(ledbit > 7 || command > 1)
		return;
	
	Led.bit_temp = (~Led.status & (0x01 << ledbit));
	
	if(Led.bit_temp == 0x00 && command == ON)
	{
		Led.status &= ~(0x01 << ledbit);
		EA = 0;
		P2 |= LEDS;
		P0 = Led.status;
		P2 &= NONE;
		EA = 1;
	}
	else if(Led.bit_temp > 0x00 && command == OFF)
	{
		Led.status |= (0x01 << ledbit);
		EA = 0;
		P2 |= LEDS;
		P0 = Led.status;
		P2 &= NONE;
		EA = 1;
	}
}

void relay_or_buzzer(uchar choice, uchar command)
{
	if((choice!=RELAY && choice!=BUZZ) || command > 1)
	{
		return;
	}	

	RelayOrBuzz.bit_temp = (RelayOrBuzz.status & choice);

	if(RelayOrBuzz.bit_temp == 0x00 && command == ON)
	{
		RelayOrBuzz.status |= choice;
		EA = 0;
		P2 |= OTHS;
		P0 = RelayOrBuzz.status;
		P2 &= NONE;
		EA = 1;
	}
	else if(RelayOrBuzz.bit_temp > 0x00 && command == OFF)
	{
		RelayOrBuzz.status &= ~choice;
		EA = 0;
		P2 |= OTHS;
		P0 = RelayOrBuzz.status;
		P2 &= NONE;
		EA = 1;
	}
}
