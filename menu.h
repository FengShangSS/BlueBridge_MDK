#ifndef __MENU_H__
#define __MENU_H__
#include "sys.h"
#include "ds1302.h"
#include "onewire.h"

#define UI_TIME		 0
#define UI_TEMP 	 1
#define UI_VOLTAGE 	 2
#define UI_FREQUENCY 3
#define UI_NONE		 4

typedef struct
{
	uchar mode;
	uchar sub[5];
}MENU_t;

extern TIME_t Time;
extern TEMP_t Temperature;
extern MENU_t Menu;

void menuUpdate(MENU_t *menu);
void freshDisbuff(SMG_t *smg);

#endif
