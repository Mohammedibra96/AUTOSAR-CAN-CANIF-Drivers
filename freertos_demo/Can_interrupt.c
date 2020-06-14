#include"include/Can.h"
#include"include/Can_interrupt.h"

void Can0_InterruptHandler(void)
{
	CAN_IRQHandler(CAN_CONTROLLER_ZERO);
	
}

void Can1_InterruptHandler(void) 
{
	CAN_IRQHandler(CAN_CONTROLLER_ONE);

}
