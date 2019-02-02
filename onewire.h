#ifndef _ONEWIRE_H
#define _ONEWIRE_H

#include "sys.h"

#define OW_SKIP_ROM 0xcc
#define DS18B20_CONVERT 0x44
#define DS18B20_READ 0xbe

typedef struct
{
	uchar high;
	uchar low;
	uchar sign;
	uint16 fraction_temp;
	uchar integer;
	uchar fraction;
	
	uchar scanCount;
	uchar timeOk;
}TEMP_t;


//IC引脚定义
sbit DQ = P1^4;

//函数声明
void Delay_OneWire(unsigned int t);
void Write_DS18B20(unsigned char dat);
bit Init_DS18B20(void);
unsigned char Read_DS18B20(void);
void temp_read(TEMP_t *temperature);

#endif