#include <Can.h>
#include <MCU.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "utils/uartstdio.h"

#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//
//
//#ifdef DEBUG
//void
//__error__(char *pcFilename, uint32_t ui32Line)
//{
//}
//
//#endif
//
//
//
//
//extern void Can_Mcu_Init(void);
//extern void
//SimpleDelay(void);
//extern void LED_Init(void);
//extern void LED_ON(void);
//
//
//void Write_Task(void * pv);
//void Read_Task(void * pv) ;
//void WriteData_Task(void * pv) ;
//extern void InitConsole(void);
//
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    while(1)
    {
    }
}
//
//
//int
//main(void)
//{
//    SysCtlPeripheralReset(SYSCTL_PERIPH_CAN0);
//    Can_Mcu_Init();
//    InitConsole();
//    UARTprintf("Welcome \n");
//
//    uint8_t Data[]={1,2,3,4,5,6,7,8};
//
//    Can_PduType Can_Pdu ={
//                          .swPduHandle= 5,
//                          .length = 8    ,
//                          .id     = 0    ,
//                          .sdu    = Data
//    };
//    Can_Init(0);
//
//    Can_DisableControllerInterrupts(CAN_CONTROLLER_ZERO);
//    Can_SetControllerMode(CAN_CONTROLLER_ZERO , CAN_T_START );
//
//    xTaskCreate(Read_Task,NULL,85,NULL,1,NULL);
//
//    /**
//    xTaskCreate(WriteData_Task,NULL,85,NULL,2,NULL);
//    xTaskCreate(Write_Task,NULL,85,NULL,1,NULL);
//     */
//
//
//    vTaskStartScheduler();
//
//    while(1)
//    {
//    }
//
//
//}
//
//void Read_Task(void * pv)
//{
//    (void)pv;
//    while(1)
//    {
//        Can_MainFunction_Read();
//        vTaskDelay(40);
//    }
//}
//
//void Write_Task(void * pv)
//{
//
//    (void)pv;
//    while(1)
//    {
//        Can_MainFunction_Write();
//        vTaskDelay(20);
//    }
//}
//
//
//
//
//void WriteData_Task(void * pv)
//{
//
//    (void)pv;
//    while(1)
//    {
//        uint8_t Data[]={1,2,3,4,5,6,7,8};
//
//        Can_PduType Can_Pdu ={
//                              .swPduHandle= 5,
//                              .length = 8    ,
//                              .id     = 0    ,
//                              .sdu    = Data
//        };
//
//        Can_Write(0,&Can_Pdu);
//        vTaskDelay(40);
//    }
//}
//
//
//
//
//
