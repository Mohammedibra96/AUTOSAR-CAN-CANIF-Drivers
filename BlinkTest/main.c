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

#include"include/Can.h"
#include"include/MCU.h"


void Can_Mcu_Init(void);
void Can_GPIO_Init(void);

int main(void)
{

    uint8_t Data[]={1,2};

    Can_PduType Can_Pdu ={
                          .swPduHandle= 5,
                          .length = 2    ,
                          .id     = 0    ,
                          .sdu    = Data

    };
    Can_Mcu_Init();
    Can_GPIO_Init();
    Can_Init(0);



    Can_SetControllerMode(CAN_CONTROLLER_ZERO , CAN_CS_STARTED );
    Can_Write( 0 , &Can_Pdu);
    while(1)
    {




    }
    return 0;


}

void Can_Mcu_Init(void)
{
    // For this example CAN0 is used with RX and TX pins on port B4 and B5.
    // The actual port and pins used may be different on your part, consult
    // the data sheet for more information.
    // GPIO port B needs to be enabled so these pins can be used.
    // TODO: change this to whichever GPIO port you are using
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

}

void Can_GPIO_Init(void)
{
    //
    // Configure the GPIO pin muxing to select CAN0 functions for these pins.
    // This step selects which alternate function is available for these pins.
    // This is necessary if your part supports GPIO pin function muxing.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using
    //
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);

    //
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
    //UARTStdioConfig(0, 115200, 16000000);
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

