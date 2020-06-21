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


#include <MCU.h>
#include <Can.h>
#include "CanIf.h"




#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif

#define MAIN_FUNCTION FALSE

void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    while(1)
    {
    }
}

void Write_Task(void * pv);
void Read_Task(void * pv) ;
void WriteData_Task(void * pv) ;

void Can_Mcu_Init(void);
void
SimpleDelay(void);
void LED_Init(void);
void LED_ON(void);
void InitConsole(void);


uint8_t ErrorState ;

uint8_t Data[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};

VAR(PduInfoType,AUTOMATIC) PduInfoPtr[] =
{
 {
  .MetaDataPtr = NULL,
  .SduDataPtr  = &Data[0],
  .SduLength   = 2
 },
 {
  .MetaDataPtr = NULL,
  .SduDataPtr  = &Data[2],
  .SduLength   = 2
 },
 {
  .MetaDataPtr = NULL,
  .SduDataPtr  = &Data[4],
  .SduLength   = 2
 },
 {
  .MetaDataPtr = NULL,
  .SduDataPtr  = &Data[6],
  .SduLength   = 2
 }
};


const Can_PduType Can_Pdu[4]={
                              {
                               .swPduHandle = 0    ,
                               .length      = 2    ,
                               .id          = 8    ,
                               .sdu         = &Data[0]
                              },{

                                 .swPduHandle = 1    ,
                                 .length      = 2    ,
                                 .id          = 7    ,
                                 .sdu         = &Data[2]
                              },
                              {
                               .swPduHandle = 5    ,
                               .length      = 2    ,
                               .id          = 2    ,
                               .sdu         = &Data[4]
                              },
                              {
                               .swPduHandle = 5    ,
                               .length      = 2    ,
                               .id          = 3    ,
                               .sdu         = &Data[6]
                              }
};




uint32_t ui32Loop;
int main(void)
{

    SysCtlPeripheralReset(SYSCTL_PERIPH_CAN0);
    LED_Init();

    Can_Mcu_Init();
    InitConsole() ;
    Can_Init(0)   ;
    CanIf_Init(0) ;
    //    Can_SetBaudrate(0, 1);
    uint8_t ErrorState ;

    UARTprintf("Welcome\n");

    //        Can_DisableControllerInterrupts(CAN_CONTROLLER_ZERO);
    //        Can_DisableControllerInterrupts(CAN_CONTROLLER_ZERO);
#if MAIN_FUNCTION == TRUE
    Can_DisableControllerInterrupts(CAN_CONTROLLER_ZERO);
#endif
    //        Can_EnableControllerInterrupts(CAN_CONTROLLER_ZERO);
    //        Can_EnableControllerInterrupts(CAN_CONTROLLER_ZERO);
    //        Can_EnableControllerInterrupts(CAN_CONTROLLER_ZERO);
    //        Can_EnableControllerInterrupts(CAN_CONTROLLER_ZERO);

    Can_SetControllerMode( CAN_CONTROLLER_ONE , CAN_CS_STARTED );


    CanIf_SetPduMode(0, CANIF_OFFLINE);
    CanIf_SetPduMode(1, CANIF_ONLINE);
    CanIf_SetPduMode(2, CANIF_OFFLINE);
    CanIf_SetPduMode(4, CANIF_ONLINE);

    CanIf_SetPduMode(3, CANIF_ONLINE);
    // Can_SetBaudrate(CAN_CONTROLLER_ZERO ,1 );

    //    Can_SetBaudrate(CAN_CONTROLLER_ZERO ,2 );


#if MAIN_FUNCTION == TRUE

    xTaskCreate(Read_Task,NULL,0x200,NULL,1,NULL);
//    xTaskCreate(WriteData_Task,NULL,0x200,NULL,2,NULL);
//    xTaskCreate(Write_Task,NULL,0x200,NULL,3,NULL);

    vTaskStartScheduler();


    while(1)
    {

    }

#endif
    while(1)
    {
        uint8_t i ;
        for ( i = 0 ; i<4 ; i++)
        {

            CanIf_Transmit(i, &PduInfoPtr[i]);
            SimpleDelay();
            Can_GetControllerErrorState( 1,&ErrorState );
            UARTprintf("Error status = %x",ErrorState)  ;

        }
    }
    return 0;
}

void Can_Mcu_Init(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
    //
    // Set up the serial console to use for displaying messages.  This is
    // just for this example program and is not needed for CAN operation.
    //
    //InitConsole();

    //
    // For this example CAN0 is used with RX and TX pins on port B4 and B5.
    // The actual port and pins used may be different on your part, consult
    // the data sheet for more information.
    // GPIO port B needs to be enabled so these pins can be used.
    // TODO: change this to whichever GPIO port you are using
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    // Configure the GPIO pin muxing to select CAN0 functions for these pins.
    // This step selects which alternate function is available for these pins.
    // This is necessary if your part supports GPIO pin function muxing.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using

    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);

    // Enable the alternate function on the GPIO pins.  The above step selects
    // which alternate function is available.  This step actually enables the
    // alternate function instead of GPIO for these pins.
    // TODO: change this to match the port/pin you are using
    //
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    // The GPIO port and pins have been set up for CAN.  The CAN peripheral
    // must be enabled.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN1);

    //
    // Initialize the CAN controller
    //
    //CANInit(CAN0_BASE);

    //
    // Set up the bit rate for the CAN bus.  This function sets up the CAN
    // bus timing for a nominal configuration.  You can achieve more control
    // over the CAN bus timing by using the function CANBitTimingSet() instead
    // of this one, if needed.
    // In this example, the CAN bus is set to 500 kHz.  In the function below,
    // the call to SysCtlClockGet() or ui32SysClock is used to determine the
    // clock rate that is used for clocking the CAN peripheral.  This can be
    // replaced with a  fixed value if you know the value of the system clock,
    // saving the extra function call.  For some parts, the CAN peripheral is
    // clocked by a fixed 8 MHz regardless of the system clock in which case
    // the call to SysCtlClockGet() or ui32SysClock should be replaced with
    // 8000000.  Consult the data sheet for more information about CAN
    // peripheral clocking.

}



void
InitConsole(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

//*****************************************************************************
//
// This function provides a 1 second delay using a simple polling method.
//
//*****************************************************************************
void
SimpleDelay(void)
{
    //
    // Delay cycles for 1 second
    //
    SysCtlDelay(16000000 / 3);
}


void LED_Init(void)
{
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    //
    // Enable the GPIO pin for the LED (PF3).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

}


void LED_ON(void)
{
    static uint8_t Flag = 0;
    if (Flag == 0)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        Flag =1;
    }
    else{
        Flag = 0;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
    }

}



#if MAIN_FUNCTION == TRUE

void Read_Task(void * pv)
{
    (void)pv;
    while(1)
    {
        Can_MainFunction_Read();
        vTaskDelay(40);
    }
}

void Write_Task(void * pv)
{

    (void)pv;
    while(1)
    {
        Can_MainFunction_Write();
        vTaskDelay(20);
    }
}

void WriteData_Task(void * pv)
{
    (void)pv;
    while(1)
    {
        uint8_t i ;
        for ( i = 0 ; i<2 ; i++)
        {
            CanIf_Transmit(i, &PduInfoPtr[i]);
            vTaskDelay(40);
            Can_GetControllerErrorState( 0,&ErrorState );
            UARTprintf("Error status = %x",ErrorState)  ;

        }
    }
}

#endif





