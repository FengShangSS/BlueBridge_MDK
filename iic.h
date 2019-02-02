#ifndef _IIC_H
#define _IIC_H

#include "sys.h"
#include "intrins.h"

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();\
				_nop_();_nop_();_nop_();_nop_();_nop_();\
				_nop_();_nop_();_nop_();_nop_();_nop_();\
				_nop_();_nop_();_nop_();_nop_();_nop_();\
				_nop_();_nop_();_nop_();_nop_();_nop_();\
				_nop_();_nop_();_nop_();_nop_();_nop_();\
				_nop_();_nop_();_nop_();_nop_();_nop_();}

#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */

//函数声明
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(unsigned char ackbit); 
void IIC_SendByte(unsigned char byt); 
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void); 
				

typedef struct
{
	uchar dat[8];
}EEPROM_t;

typedef struct
{
	uchar scanCount;
	uchar timeOk;
	uchar read;
	uchar integer;
	uchar fraction;
	float f_temp;
}ADC_t;

void operate_delay(uchar t);
void eeprom_SingleWrite(uchar add, uchar dat);
uchar eeprom_SingleRead(uchar add);	
void adc_Init(void);
uchar adc_read(void);
void dac_write(uchar dat);

#endif