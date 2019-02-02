/*
  程序说明: IIC总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台(12MHz)
  日    期: 2011-8-9
*/

#include "iic.h"

void operate_delay(uchar t)
{
	uchar i;
	
	while(t--){
		for(i=0; i<112; i++);
	}
}

void eeprom_SingleWrite(uchar add, uchar dat)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
	operate_delay(10);
}

uchar eeprom_SingleRead(uchar add)
{
	uchar dat;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	dat = IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
	return dat;
}

void adc_Init(void)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);//ADC通道3
	IIC_WaitAck();
	IIC_Stop();
	operate_delay(10);
}

uchar adc_read(void)
{
	uchar temp;

	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	temp = IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
	
	return temp;
}

void dac_write(uchar dat)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x40);//ADC通道3
	IIC_WaitAck();
	IIC_SendByte(dat);//ADC通道3
	IIC_WaitAck();
	IIC_Stop();
	operate_delay(10);
}

//总线启动条件
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	somenop;
	SDA = 0;
	somenop;
	SCL = 0;	
}

//总线停止条件
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	somenop;
	SDA = 1;
}

//应答位控制
void IIC_Ack(unsigned char ackbit)
{
	if(ackbit) 
	{	
		SDA = 0;
	}
	else 
	{
		SDA = 1;
	}
	somenop;
	SCL = 1;
	somenop;
	SCL = 0;
	SDA = 1; 
	somenop;
}

//等待应答
bit IIC_WaitAck(void)
{
	SDA = 1;
	somenop;
	SCL = 1;
	somenop;
	if(SDA)    
	{   
		SCL = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		SCL = 0;
		return 1;
	}
}

//通过I2C总线发送数据
void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	
	EA = 0;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		somenop;
		SCL = 1;
		byt <<= 1;
		somenop;
		SCL = 0;
	}
	EA = 1;
}

//从I2C总线上接收数据
unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;
	
	EA = 0;
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		somenop;
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		somenop;
	}
	EA = 1;

	return da;
}