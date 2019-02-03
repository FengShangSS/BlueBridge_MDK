#include "sonic.h"

SONIC_t Sonic;

void send_wave(void)
{
	unsigned char i = 8;  //����8������
	
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
	TR1 = 1;  //������ʱ
	while((RX == 1) && (TF1 == 0));  //�ȴ��յ�����
	TR1 = 0;  //�رռ�ʱ

	//�������
	if(TF1 == 1)
	{
		TF1 = 0;
		sonic->distance = 999;  //�޷���
	}
	else
	{
		/**  ����ʱ��  */
		sonic->read = TH1;
		sonic->read <<= 8;
		sonic->read |= TL1;
		sonic->distance = (unsigned short)(sonic->read*0.015);  //�������				
	}
	TH1 = 0;
	TL1 = 0;
}