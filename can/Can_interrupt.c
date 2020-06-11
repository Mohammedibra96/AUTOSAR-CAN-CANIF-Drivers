#include "std_types.h"


/*	TODO: move these two prototypes tp the can_interrupt.h	*/
#define ENTER_CRITICAL_SECTION()	
#define EXIT_CRITICAL_SECTION()	

void Can0_InterruptHandler(void);
void Can1_InterruptHandler(void);


void Can0_InterruptHandler(void)
{
	CAN_IRQHandler(CAN_CONTROLLER_ZERO);
	
}

void Can1_InterruptHandler(void) 
{
	CAN_IRQHandler(CAN_CONTROLLER_ONE);

}
