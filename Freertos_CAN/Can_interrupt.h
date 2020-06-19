#include "std_types.h"


#define ENTER_CAN_CONTROLLER_INTERRUPT_DISABLE(CONTROLLER)				do{CanControllerInterruptDisable[CONTROLLER]++;}while(0)


#define EXIT_CAN_CONTROLLER_INTERRUPT_DISABLE(CONTROLLER)				do{CanControllerInterruptDisable[CONTROLLER]--;}while(0)


#define IN_CAN_CONTROLLER_INTERRUPT_DISABLE()							(CanControllerInterruptDisable[Controller] != CAN_DISABLE_INTERRUPT_NOT_CALLED)


#define OUT_CAN_CONTROLLER_INTERRUPT_DISABLE()							(CanControllerInterruptDisable[Controller] == CAN_DISABLE_INTERRUPT_NOT_CALLED)


FUNC(void,CAN_CODE) CAN_IRQHandler(VAR(uint8_t, AUTOMATIC) Controller);

void Can0_InterruptHandler(void);
void Can1_InterruptHandler(void);

