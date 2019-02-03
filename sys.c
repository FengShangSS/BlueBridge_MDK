#include "sys.h"

uchar tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,\
				0xc0-0x80,0xf9-0x80,0xa4-0x80,0xb0-0x80,0x99-0x80,\
				0x92-0x80,0x82-0x80,0xf8-0x80,0x80-0x80,0x90-0x80,\
				0xff,0xbf,0xc6,0x8E,0xC1,0xC7};

SMG_t Smg = {0, 11,11,11,11,11,11,11,11};
LED_t Led = {0xFF, 0x00, 0,0,0, 0,0,0};
LED_t RelayOrBuzz = {0x00, 0x00, 0,0,0, 0,0,0};
TIME1FLAG_t Time1Flag = {0x00, 0x00};

void sysInit(void)
{
	IOinit();
	Timer0Init();
//	Timer2Init();
	UartInit();
	ES = 1;
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

void Timer0Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x9A;		//���ö�ʱ��ֵ
	TH0 = 0xA9;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;		//ʹ�ܶ�ʱ��0�ж�
}


void Timer1Init(uchar command)		//2����@11.0592MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
//	TL1 = 0x9A;		//���ö�ʱ��ֵ
//	TH1 = 0xA9;		//���ö�ʱ��ֵ
//	TF1 = 0;		//���TF1��־
//	TR1 = 1;		//��ʱ��1��ʼ��ʱ
//	ET1 = 1;		//ʹ�ܶ�ʱ��1�ж�
	if(command == TIME1FRE)
	{
//		AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
//		TMOD &= 0x0F;
		TMOD |= 0x50;	//���ö�ʱ��ģʽ
		IT1 = 1;		//�½��ش���
		TL1 = 0x00;		//���ö�ʱ��ֵ
		TH1 = 0x00;		//���ö�ʱ��ֵ
		TR1 = 1;		//��ʱ��1��ʼ��ʱ
		Time1Flag.done = TIME1FRE;
		led_control(2, ON);
		led_control(3, OFF);
	}
	else if(command == TIME1LEN)
	{
//		AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
//		TMOD &= 0x0F;
		TMOD |= 0x10;	//���ö�ʱ��ģʽ
		TL1 = 0x00;		//���ö�ʱ��ֵ
		TH1 = 0x00;		//���ö�ʱ��ֵ
		Time1Flag.done = TIME1LEN;
		led_control(2, OFF);
		led_control(3, ON);
	}
	else
	{
		Time1Flag.done = 0;
		led_control(2, OFF);
		led_control(3, OFF);
	}	
}

void Timer2Init(void)		//@11.0592MHz//���벶��
{
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	AUXR |= 0x08;
	T2L = 0x00;		//���ö�ʱ��ֵ
	T2H = 0x00;		//���ö�ʱ��ֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
}

void Timer2ReInit(void)//���벶��
{
	AUXR &= ~0x10;	//�ؼ�ʱ
	T2L = 0x00;		//���ö�ʱ��ֵ
	T2H = 0x00;		//���ö�ʱ��ֵ
	AUXR |= 0x10;	//����ʱ
}

void UartInit(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xE0;		//�趨��ʱ��ֵ
	T2H = 0xFE;		//�趨��ʱ��ֵ
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x10;		//������ʱ��2
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

void trans_char(char t)
{
	SBUF = t;
	while(!TI);
	TI = 0;
}
 
void trans_str(char t[])
{
	char *p;
	p = t;
	while(*p != '\0')
	{
		trans_char(*p);
		p++;
	}	
}

