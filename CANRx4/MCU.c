#include"include/Std_Types.h"
#include "driverlib/sysctl.h"

#include "include/MCU.h"



uint32_t Mcu_GetSysCloock(void)
{
	return SysCtlClockGet();
}
