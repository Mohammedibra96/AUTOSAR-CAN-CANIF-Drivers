#include "std_types.h"


/*	TODO: move these two prototypes tp the can_interrupt.h	*/
#define ENTER_CRITICAL_SECTION(CONTROLLER)				do{IntDisableCount[CONTROLLER]++;}while(0)
	
#define EXIT_CRITICAL_SECTION(CONTROLLER)				do{IntDisableCount[CONTROLLER]--;}while(0)

#define IN_CRITICAL_SECTION()							(IntDisableCount[Controller] != CAN_DISABLE_INTERRUPT_NOT_CALLED)
#define OUT_CRITICAL_SECTION()							(IntDisableCount[Controller] == CAN_DISABLE_INTERRUPT_NOT_CALLED)


FUNC(void,CAN_CODE) CAN_IRQHandler(VAR(uint8_t, AUTOMATIC) Controller);

void Can0_InterruptHandler(void);
void Can1_InterruptHandler(void);

