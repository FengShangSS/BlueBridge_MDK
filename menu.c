#include "menu.h"

TIME_t Time = {55,52,0, 1,2, 5, 19,\
				0,0,0, 0,0, 0, 0,\
				0};
TEMP_t Temperature;
MENU_t Menu = {UI_AUTO, 0,0,0,0,0, 0};
EEPROM_t Eeprom;
ADC_t Adc;
FRE_t Fre;

void menuUpdate(MENU_t *menu)
{	
	if(menu->sub[UI_TIME] == 1)
	{
		if(Time.runFlag)
			Ds_Control(&Time, STOP);
	}
	else
	{
		if(!Time.runFlag)
			Ds_Control(&Time, RUN);
	}
	
	if(menu->mode == UI_AUTO)
	{
		menu->autoFlag = 1;
	}

	if(menu->autoFlag == 1)
	{
		led_control(1, ON);
		if(Time.read[0] >= 1 && Time.read[0] <= 3)
		{
			Menu.mode = UI_TEMP;
		}
		else
		{
			Menu.mode = UI_TIME;
		}
	}
	else
		led_control(1, OFF);
	
	freshDisbuff(&Smg);
}

void freshDisbuff(SMG_t *smg)
{
	//timeUpdate
	if(Menu.mode == UI_TIME)
	{
		smg->disbuff[0] = Time.read[2] / 10;
		smg->disbuff[1] = Time.read[2] % 10;
		smg->disbuff[2] = DIS_SHORT_HORIZON;
		smg->disbuff[3] = Time.read[1] / 10;
		smg->disbuff[4] = Time.read[1] % 10;
		smg->disbuff[5] = DIS_SHORT_HORIZON;
		smg->disbuff[6] = Time.read[0] / 10;
		smg->disbuff[7] = Time.read[0] % 10;
	}	
	//temperatureUpdate
	else if(Menu.mode == UI_TEMP)
	{
		smg->disbuff[0] = DIS_C;
		smg->disbuff[1] = DIS_NONE;
		smg->disbuff[2] = DIS_NONE;
		smg->disbuff[3] = (Temperature.sign) ? DIS_SHORT_HORIZON : DIS_NONE;
//		smg->disbuff[4] = Temperature.integer / 100;
//		(Temperature.integer >= 100) ? (smg->disbuff[4] = Temperature.integer / 100) : DIS_NONE;
		smg->disbuff[4] = (Temperature.integer / 100 == 0) ? DIS_NONE : Temperature.integer / 100;
		(Temperature.integer >= 10) ? (smg->disbuff[5] = Temperature.integer % 100 / 10) : DIS_NONE;
		smg->disbuff[6] = Temperature.integer % 10 + DIS_POINT;
		smg->disbuff[7] = Temperature.fraction;
	}
	//voltageUpdate
	else if(Menu.mode == UI_VOLTAGE)
	{
		smg->disbuff[0] = DIS_U;
		smg->disbuff[1] = DIS_NONE;
		smg->disbuff[2] = DIS_NONE;
		smg->disbuff[3] = DIS_NONE;
		smg->disbuff[4] = DIS_NONE;
		smg->disbuff[5] = Adc.integer + DIS_POINT;
		smg->disbuff[6] = Adc.fraction / 10;
		smg->disbuff[7] = Adc.fraction % 10;
	}
	//frequencyUpdate
	else if(Menu.mode == UI_FREQUENCY)
	{
		smg->disbuff[0] = DIS_F;
		smg->disbuff[1] = DIS_NONE;
		smg->disbuff[2] = DIS_NONE;
		smg->disbuff[3] = DIS_NONE;
		smg->disbuff[4] = DIS_NONE;
		smg->disbuff[5] = Fre.integer / 100;
		smg->disbuff[6] = Fre.integer % 100 / 10;
		smg->disbuff[7] = Fre.integer % 10;
	}
	//record
	else if(Menu.mode == UI_RECORD)
	{
		smg->disbuff[0] = Menu.sub[UI_RECORD];
		smg->disbuff[1] = DIS_SHORT_HORIZON;
		smg->disbuff[2] = DIS_NONE;
		smg->disbuff[3] = DIS_NONE;
		smg->disbuff[4] = DIS_NONE;
		smg->disbuff[5] = Eeprom.dat[Menu.sub[UI_RECORD]] / 100;
		smg->disbuff[6] = Eeprom.dat[Menu.sub[UI_RECORD]] % 100 / 10;
		smg->disbuff[7] = Eeprom.dat[Menu.sub[UI_RECORD]] % 10;
	}
	//AUTO
	//none
	else
	{
//		smg->disbuff[0] = (Eeprom.dat[1] / 100 == 0) ? DIS_NONE : Eeprom.dat[1] / 100;
//		smg->disbuff[1] = (Eeprom.dat[1] % 100 / 10 == 0) ? DIS_NONE : Eeprom.dat[1] % 100 / 10;
//		smg->disbuff[2] = Eeprom.dat[1] % 10;
//		smg->disbuff[3] = DIS_NONE;
//		smg->disbuff[4] = (Eeprom.dat[0] / 100 == 0) ? DIS_NONE : Eeprom.dat[0] / 100;
//		smg->disbuff[5] = (Eeprom.dat[0] % 100 / 10 == 0) ? DIS_NONE : Eeprom.dat[0] % 100 / 10;
//		smg->disbuff[6] = Eeprom.dat[0] % 10;
//		smg->disbuff[7] = DIS_NONE;
		
		smg->disbuff[0] = DIS_NONE;
		smg->disbuff[1] = DIS_NONE;
		smg->disbuff[2] = DIS_NONE;
		smg->disbuff[3] = DIS_NONE;
		smg->disbuff[4] = DIS_NONE;
		smg->disbuff[5] = DIS_NONE;
		smg->disbuff[6] = DIS_NONE;
		smg->disbuff[7] = DIS_NONE;
	}

}
