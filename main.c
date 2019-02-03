#include "menu.h"
#include "keyscan.h"

/*------------------------------------------------------------------
-------------------------V0.0.1-----------------------------------
DATE:2019.1.31		TIME:21:04		AUTHOR:FS
USAGE:
Smg.disbuff[]
led_control(uchar ledbit, uchar command)
relay_or_buzzer(uchar choice, uchar command)

-------------------------V0.0.2-----------------------------------
DATE:2019.2.01		TIME:19:31		AUTHOR:FS
USAGE:
Key
keyScan()
keyProcess(KEY_t *in, MENU_t *menu)

-------------------------V0.0.3-----------------------------------
DATE:2019.2.01		TIME:22:24		AUTHOR:FS
USAGE:
Time.set[7]
Time.read[7]
Ds_Control(TIME_t *time, uchar command)

-------------------------V0.0.4-----------------------------------
DATE:2019.2.02		TIME:15:37		AUTHOR:FS
USAGE:
Temperature.integer
Temperature.fraction
temp_read(TEMP_t *temperature)

Menu.mode
Menu.sub[]
menuUpdate(MENU_t *menu)
freshDisbuff(SMG_t *smg)

-------------------------V0.0.5-----------------------------------
DATE:2019.2.03		TIME:00:40		AUTHOR:FS
USAGE:
eeprom_SingleWrite(uchar add, uchar dat);
eeprom_SingleRead(uchar add);	
adc_Init(void);
adc_read(void);
dac_write(uchar dat);

-------------------------V0.0.6-----------------------------------
DATE:2019.2.03		TIME:21:49		AUTHOR:FS
USAGE:
send_wave();
read_distance(&Sonic);

-------------------------------------------------------------------*/


uint systick;
uchar index;
char rechar;
uint16 pwmCount;

void main()
{
	sysInit();
	Ds_Control(&Time, WRITE);
	Eeprom.dat[0] = eeprom_SingleRead(0x00);
	Eeprom.dat[1] = eeprom_SingleRead(0x01);
	Eeprom.dat[2] = eeprom_SingleRead(0x02);
	Eeprom.dat[3] = eeprom_SingleRead(0x03);
	Eeprom.dat[4] = eeprom_SingleRead(0x04);
	
	while(1)
	{
		if(Sonic.timeOk == 1)
		{
			Sonic.timeOk = 0;
			send_wave();
			read_distance(&Sonic);
		}
		
		if(Fre.timeOk == 1)
		{
			Fre.timeOk = 0;
			Fre.high = TH1;
			Fre.low = TL1;
			Fre.integer = Fre.high * 256 + Fre.low;
			TH1 = TL1 = 0;
		}
		
		if(Adc.timeOk == 1)
		{
			Adc.timeOk = 0;
			adc_Init();
			Adc.read = adc_read();
			dac_write(Adc.read);
			Adc.f_temp = Adc.read / 255. * 5;
			Adc.integer = Adc.f_temp;
			Adc.fraction = (Adc.f_temp - Adc.integer) * 100;
		}
		
		if(Temperature.timeOk == 1)
		{
//			uchar i;
			Temperature.timeOk = 0;
			temp_read(&Temperature);
//			for(i = 0; i < 4; i++)
//				Temperature.integer_temp[i] = temp_read(&Temperature);
//			
//			Temperature.integer_temp[4] = Temperature.integer_temp[0] + Temperature.integer_temp[1] +\
//											Temperature.integer_temp[2] + Temperature.integer_temp[3];
//			
//			Temperature.integer_temp[4] /= 4;
//			
//			Temperature.integer = Temperature.integer_temp[4];
//			
//			if(systick < 500 * 10)
//				Temperature.integer_last = Temperature.integer;
//			else
//			{
//				if(Temperature.integer_last - Temperature.integer < -2 || Temperature.integer_last - Temperature.integer > 2)
//					Temperature.integer = Temperature.integer_last;
//				
////				if(Temperature.integer <= 20)
////					relay_or_buzzer(RELAY, ON);
////				else
////					relay_or_buzzer(RELAY, OFF);
//				
//				Temperature.integer_last = Temperature.integer;
//			}
		}
		
		if(Key.timeOk == 1)
		{
			Key.timeOk = 0;
			keyScan();
			keyProcess(&Key, &Menu);
		}
		
		if(Time.runFlag)
		{
			Ds_Control(&Time, READ);
			if(Time.read[0] == 59 && Time.read[1] == 59)
			{
				switch(Time.read[2])
				{
					case 1:
						eeprom_SingleWrite(0x00, Temperature.integer);
						break;
					case 2:
						eeprom_SingleWrite(0x01, Temperature.integer);
						break;
					case 3:
						eeprom_SingleWrite(0x02, Temperature.integer);
						break;
					case 4:
						eeprom_SingleWrite(0x03, Temperature.integer);
						break;
					case 5:
						eeprom_SingleWrite(0x04, Temperature.integer);
						break;
					default:
						break;
				}
				
			}
		}
		
		menuUpdate(&Menu);
	}
}
/*

*/
void timer0() interrupt 1//2ms
{
	systick++;
	
	if(pwmCount <= 10)
	{
		pwmCount++;
		if(pwmCount <= 1)
			led_control(5, ON);
		else
			led_control(5, OFF);
		
		if(pwmCount <= 3)
			led_control(6, ON);
		else
			led_control(6, OFF);
	}
	else
		pwmCount = 0;
	
	if(++Sonic.scanCount >= 200)//0.4s
	{
		Sonic.scanCount = 0;
		if(Time1Flag.done == TIME1LEN)
			Sonic.timeOk = 1;
		else
			Sonic.timeOk = 0;
	}
	
	if(++Fre.count >= 500)//1s
	{
		Fre.count = 0;
		if(Time1Flag.done == TIME1FRE)
			Fre.timeOk = 1;
		else
			Fre.timeOk = 0;
	}
	
	if(++Adc.scanCount >= 100)//0.2s
	{
		Adc.scanCount = 0;
		Adc.timeOk = 1;
	}
	
	if(++Temperature.scanCount >= 200)//0.4s
	{
		Temperature.scanCount = 0;
		Temperature.timeOk = 1;
	}
	
	if(++Key.sacnCount >= 5)//0.01s
	{
		P12 = ~P12;
		Key.sacnCount = 0;
		Key.timeOk = 1;
	}
	
	if(Key.mode == LONG)
	{
		led_control(7, ON);
		if(++Key.longPressCount >= 125)//0.25s
		{
			Key.longPressCount = 0;
			Key.longPressFlag = 1;
		}
	}
	else if(Key.mode == SHORT)
	{
		led_control(7, OFF);
		Key.longPressCount = 0;
		Key.longPressFlag = 0;
	}
	
	if(++Led.count[0] >= 250)//0.5s
	{
		Led.count[0] = 0;
		Led.flag[0]++;
		if(Led.flag[0]&0x01)
		{
			led_control(0, ON);
//			relay_or_buzzer(RELAY, ON);
//			relay_or_buzzer(BUZZ, OFF);
		}
		else
		{
			led_control(0, OFF);
//			relay_or_buzzer(RELAY, OFF);
//			relay_or_buzzer(BUZZ, ON);
		}
	}
	display();
}

void uart_isr() interrupt 4
{	
	led_control(4, ON);
	RI = 0;
	rechar = SBUF;
	trans_char(rechar);	  	//—È÷§ 
}