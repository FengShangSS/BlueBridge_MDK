#ifndef __SONIC_H__
#define __SONIC_H__
#include "sys.h"
#include "intrins.h"

#define SNOP {_nop_();_nop_();_nop_();_nop_();_nop_();\
                 _nop_();_nop_();_nop_();_nop_(); _nop_();}


sbit TX = P1^0;  //发射引脚
sbit RX = P1^1;  //接收引脚	

typedef struct
{
	float speed;
	uint16 read;
	uint16 distance;
	uint16 scanCount;
	uchar timeOk;
}SONIC_t;	
		
extern SONIC_t Sonic;

void send_wave(void);
void read_distance(SONIC_t *sonic);
				
#endif
