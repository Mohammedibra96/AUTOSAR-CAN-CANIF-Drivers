#include <Std_Types.h>
#include <MCU.h>
#include "driverlib/sysctl.h"




uint32_t Mcu_GetSysCloock(void)
{
	return SysCtlClockGet();
}
