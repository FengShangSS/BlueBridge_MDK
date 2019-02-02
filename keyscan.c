#include "keyscan.h"

KEY_t Key;

void keyScan(void)
{
	Key.currRead = P3;
	
	//无按键按下
	if(Key.currRead == 0xFF)
	{
		Key.Release = Key.lastRead;
		Key.Down = 0xFF;
		Key.Continue = 0xFF;
		Key.validCount = 0;
		Key.pressTimeCount = 0;
		Key.mode = SHORT;
		Key.last = 0xFF;
		return;
	}
	
	//有按键按下
	if(Key.currRead == Key.lastRead)
	{
		if(Key.validCount >= KEYDEBOUNCE)
		{
			//按住不放多次响应
			Key.curr = Key.currRead;
			Key.Continue = Key.curr;
			
			//按住不放只响应一次
			if(Key.last == Key.curr)
			{
				Key.Down = 0xFF;
				Key.pressTimeCount++;
				if(Key.pressTimeCount >= 20)
				{
					Key.pressTimeCount = 0;
					Key.mode = LONG;
				}
			}
			else
			{
				Key.Down = Key.curr;
				Key.mode = SHORT;
			}
			
			Key.last = Key.curr;
		}
		else
			Key.validCount++;
	}
	else//按键发生改变
	{
		Key.lastRead = Key.currRead;
	}
}

void keyProcess(KEY_t *in, MENU_t *menu)
{
	switch(in->mode)
	{
		case LONG:
			if(Key.longPressFlag == 1)
			{
				Key.longPressFlag = 0;
				switch(in->Continue)
				{
					case 0xFE:
						menu->sub[menu->mode]++;
						break;
					case 0xFD:
						menu->sub[menu->mode]--;
						break;
					case 0xFB:
						menu->sub[menu->mode] = 0;
						break;
					case 0xF7:
						menu->mode++;
						if(menu->mode >= 5)
							menu->mode = 0;
						break;
					case 0xFF:
					default:
						break;
				}
			}
			break;
		
		case SHORT:
			switch(in->Down)
			{
				case 0xFE:
					menu->sub[menu->mode]++;
					in->Down = 0xFF;
					break;
				case 0xFD:
					menu->sub[menu->mode]--;
					in->Down = 0xFF;
					break;
				case 0xFB:
					menu->sub[menu->mode] = 0;
					in->Down = 0xFF;
					break;
				case 0xF7:
					menu->mode++;
					if(menu->mode >= 5)
						menu->mode = 0;
					in->Down = 0xFF;
					break;
				case 0xFF:
				default:
					break;
			}
			break;
		
		case RELEASE:
			switch(in->Release)
			{
				case 0xFE:
					menu->sub[menu->mode]++;
					in->lastRead = 0xFF;
					in->Release = 0xFF;
					break;
				case 0xFD:
					menu->sub[menu->mode]--;
					in->lastRead = 0xFF;
					in->Release = 0xFF;
					break;
				case 0xFB:
					menu->sub[menu->mode] = 0;
					in->lastRead = 0xFF;
					in->Release = 0xFF;
					break;
				case 0xF7:
					menu->mode++;
					if(menu->mode >= 5)
						menu->mode = 0;
					in->lastRead = 0xFF;
					in->Release = 0xFF;
					break;
				case 0xFF:
				default:
					break;
			}
			break;
	}
}