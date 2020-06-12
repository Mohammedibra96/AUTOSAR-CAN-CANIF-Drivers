#include"MCU.h"



uint32_t Mcu_GetSysCloock(void)
{
	SysCtlClockGet();
}