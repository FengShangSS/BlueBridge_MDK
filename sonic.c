#include "sonic.h"

SONIC_t Sonic;

void send_wave(void)
{
	unsigned char i = 8;  //发送8个脉冲
	
	do
	{
		TX = 1;
		SNOP;SNOP;SNOP;SNOP;SNOP;	SNOP;SNOP;SNOP;SNOP;SNOP;	
		TX = 0;
		SNOP;SNOP;SNOP;SNOP;SNOP;	SNOP;SNOP;SNOP;SNOP;SNOP;	
	}
	while(i--);
}

void read_distance(SONIC_t *sonic)
{
	TR1 = 1;  //启动计时
	while((RX == 1) && (TF1 == 0));  //等待收到脉冲
	TR1 = 0;  //关闭计时

	//发生溢出
	if(TF1 == 1)
	{
		TF1 = 0;
		sonic->distance = 999;  //无返回
	}
	else
	{
		/**  计算时间  */
		sonic->read = TH1;
		sonic->read <<= 8;
		sonic->read |= TL1;
		sonic->distance = (unsigned short)(sonic->read*0.015);  //计算距离				
	}
	TH1 = 0;
	TL1 = 0;
}