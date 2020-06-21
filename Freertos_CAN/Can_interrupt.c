#include <Can.h>
#include"Can_lcfg.h"
#include <Can_interrupt.h>



#define CAN_CONTROLLER_ZERO_BASE_ADDRESS 0x40040000U
#define CAN_CONTROLLER_ONE_BASE_ADDRESS  0x40041000U

void Can0_InterruptHandler(void)
{
    if ( CanController[0].CanControllerBaseAddress == CAN_CONTROLLER_ZERO_BASE_ADDRESS )
    {
        CAN_IRQHandler((uint8_t )CAN_CONTROLLER_ZERO);
    }
    else if ( CanController[1].CanControllerBaseAddress == CAN_CONTROLLER_ZERO_BASE_ADDRESS )
    {
        CAN_IRQHandler( CAN_CONTROLLER_ONE );
    }
    else
    {

    }
}

void Can1_InterruptHandler(void) 
{
    if ( CanController[0].CanControllerBaseAddress == CAN_CONTROLLER_ONE_BASE_ADDRESS )
    {
        CAN_IRQHandler((uint8_t ) CAN_CONTROLLER_ZERO );
    }
    else if  ( (MAX_NUM_CONTROLLER > 1) && (CanController[1].CanControllerBaseAddress  == CAN_CONTROLLER_ONE_BASE_ADDRESS) )
    {
        CAN_IRQHandler( (uint8_t )CAN_CONTROLLER_ONE );
    }
    else
    {

    }
}



