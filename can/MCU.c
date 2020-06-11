const Mcu_PerClockConfigType McuPerClockConfigData [NUM_OF_CAN_CONTROLLERS]= SYSCTL_PERIPH;




uint32_t Mcu_GetSysCloock(void)
{
	SysCtlClockGet();
}