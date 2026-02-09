#include "main.h"
#include "LCD.h"
#define MAXIMUM_CONTRAST 51
#define MINIMUM_CONTRAST 27
#define STEP_CONTRAST 3

void TASK_DisplayProc (void* pdata)
{
	//struct tm  tmNow;
	//static UCHAR  Sec = 0;
	//char  uc[20];
	//UCHAR uctemp;//,language;
	UNUSED(pdata);//pdata = pdata;

	//gScreenType.fSetCurrScreen(MAINSCREEN);
	//gScreenType.fDisplayScreen();//static void display_screen(void)
	
	for(;;)
	{
		gSelfTest.fSetTaskID(TASK_ID_DISPLAY);
		OSTimeDly(900);
	}
}
