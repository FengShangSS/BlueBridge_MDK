#include "menu.h"

TIME_t Time = {55,59,23, 1,2, 5, 19,\
				0,0,0, 0,0, 0, 0,\
				0};
TEMP_t Temperature;
MENU_t Menu;

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
		smg->disbuff[4] = (Temperature.integer / 100 == 0) ? DIS_NONE : Temperature.integer / 100;
		smg->disbuff[5] = (Temperature.integer % 100 / 10 == 0) ? DIS_NONE : Temperature.integer % 100 / 10;
		smg->disbuff[6] = Temperature.integer % 10 + DIS_POINT;
		smg->disbuff[7] = Temperature.fraction % 10;
	}
	//voltageUpdate
	else if(Menu.mode == UI_VOLTAGE)
	{
		smg->disbuff[0] = DIS_U;
		smg->disbuff[1] = DIS_NONE;
		smg->disbuff[2] = DIS_NONE;
		smg->disbuff[3] = DIS_NONE;
		smg->disbuff[4] = DIS_NONE;
		smg->disbuff[5] = DIS_NONE;
		smg->disbuff[6] = DIS_NONE;
		smg->disbuff[7] = DIS_NONE;
	}
	//frequencyUpdate
	else if(Menu.mode == UI_FREQUENCY)
	{
		smg->disbuff[0] = DIS_F;
		smg->disbuff[1] = DIS_NONE;
		smg->disbuff[2] = DIS_NONE;
		smg->disbuff[3] = DIS_NONE;
		smg->disbuff[4] = DIS_NONE;
		smg->disbuff[5] = DIS_NONE;
		smg->disbuff[6] = DIS_NONE;
		smg->disbuff[7] = DIS_NONE;
	}
	//none
	else if(Menu.mode == UI_NONE)
	{
		smg->disbuff[0] = DIS_NONE;
		smg->disbuff[1] = DIS_NONE;
		smg->disbuff[2] = DIS_NONE;
		smg->disbuff[3] = DIS_NONE;
		smg->disbuff[4] = DIS_NONE;
		smg->disbuff[5] = DIS_NONE;
		smg->disbuff[6] = DIS_NONE;
		smg->disbuff[7] = DIS_NONE;
	}
	//none
	else
	{
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
