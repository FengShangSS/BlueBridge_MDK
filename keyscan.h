#ifndef __KEYSCAN_H__
#define __KEYSCAN_H__
#include "sys.h"
#include "menu.h"

#define KEYDEBOUNCE 5

#define LONG		1
#define SHORT		0
#define RELEASE		2

typedef struct
{
	//初始的状态
	uchar lastRead;
	uchar currRead;
	
	//一次处理后的状态
	uchar curr;
	uchar last;
	uchar validCount;
	
	//二次处理后的状态
	uchar Down;
	uchar Release;
	uchar Continue;
		
	uchar pressTimeCount;
	uchar mode;
	
	uchar sacnCount;
	uchar timeOk;
	uint longPressCount;
	uchar longPressFlag;
}KEY_t;

extern KEY_t Key;

void keyScan(void);
void keyProcess(KEY_t *in, MENU_t *menu);
#endif
