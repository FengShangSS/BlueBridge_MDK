#ifndef __MENU_H__
#define __MENU_H__
#include "sys.h"
#include "ds1302.h"
#include "onewire.h"
#include "iic.h"

#define UI_TIME		 0
#define UI_TEMP 	 1
#define UI_VOLTAGE 	 2
#define UI_FREQUENCY 3
#define UI_RECORD 	 4
#define UI_AUTO		 5

typedef struct
{
	uchar mode;
	uchar sub[6];
	uchar autoFlag;
}MENU_t;

typedef struct
{
	uchar high;
	uchar low;
	uint16 integer;
	
	uint16 count;
	uchar timeOk;
}FRE_t;

extern TIME_t Time;
extern TEMP_t Temperature;
extern MENU_t Menu;
extern EEPROM_t Eeprom;
extern ADC_t Adc;
extern FRE_t Fre;

void menuUpdate(MENU_t *menu);
void freshDisbuff(SMG_t *smg);

#endif
