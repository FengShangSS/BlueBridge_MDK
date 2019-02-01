#ifndef __SYS_H__
#define __SYS_H__

#include <IAP15F2K61S2.H>

#define uchar unsigned char
#define uint unsigned int
#define uint16 unsigned short
	
#define NONE 0x1f
#define LEDS 0x80
#define OTHS 0xA0
#define BITS 0xC0
#define SEGS 0xE0

#define RELAY 0x10
#define BUZZ  0x40

#define ON  1
#define OFF 0

typedef struct 
{
	uchar status;
	uchar bit_temp;
}LED_t;

typedef struct 
{
	uchar disbit;
	uchar disbuff[8];
}SMG_t;

typedef struct 
{
	uchar flag0;
	uchar flag1;
	uchar flag2;
	uchar flag3;
	uchar flag4;
	uchar flag5;
	uchar flag6;
	uchar flag7;
}FLAG_t;

typedef struct 
{
	uint count0;
	uint count1;
	uint count2;
	uint count3;
	uint count4;
	uint count5;
	uint count6;
	uint count7;
}COUNT_t;

extern SMG_t Smg;

void sysInit(void);
void IOinit(void);
void Timer0Init(void);
void Timer1Init(void);
void display(void);
void led_control(uchar ledbit, uchar command);
void relay_or_buzzer(uchar choice, uchar command);

#endif
