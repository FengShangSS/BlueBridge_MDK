/*
  ����˵��: ��������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨
  ��    ��: 2011-8-9
*/

#include "onewire.h"

//��������ʱ����
void Delay_OneWire(unsigned int t)
{
	uchar i;
	while(t--)
		for(i = 0; i < 12; i++);
}

//DS18B20оƬ��ʼ��
bit Init_DS18B20(void)
{
	bit initflag = 0;
	DQ = 1;
	Delay_OneWire(12);
	DQ = 0;
	Delay_OneWire(80); 
	DQ = 1;
	Delay_OneWire(10); 
	initflag = DQ;    
	Delay_OneWire(5);
  
	return initflag;
}

//ͨ����������DS18B20дһ���ֽ�
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//��DS18B20��ȡһ���ֽ�
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

uchar temp_read(TEMP_t *temperature)
{
	if(!Init_DS18B20())
	{
		temperature->integer = 0;
		temperature->fraction = 0;
		return;
	}
	Write_DS18B20(OW_SKIP_ROM);
	Write_DS18B20(DS18B20_CONVERT);
	Delay_OneWire(5);
	
	if(!Init_DS18B20())
	{
		temperature->integer = 0;
		temperature->fraction = 0;
		return;
	}
	Write_DS18B20(OW_SKIP_ROM);
	Write_DS18B20(DS18B20_READ);
	
	temperature->low = Read_DS18B20();
	temperature->high = Read_DS18B20();
	
	temperature->integer = (temperature->high << 4) | (temperature->low >> 4);
	
	if(temperature->integer > 0x80)
	{
		temperature->sign = 1;
		temperature->integer &= 0x7F;
	}
	else
	{
		temperature->sign = 0;
	}
	
	temperature->fraction_temp = ((temperature->low&0x08)?1:0)*5000 +\
								((temperature->low&0x04)?1:0)*2500 +\
								((temperature->low&0x02)?1:0)*1250 +\
								((temperature->low&0x01)?1:0)*625;
	
	temperature->fraction = temperature->fraction_temp / 1000;
	
	return temperature->integer;
}