/* -------------------------------- Arctic Core ------------------------------
 * Arctic Core - the open source AUTOSAR platform http://arccore.com
 *
 * Copyright (C) 2009  ArcCore AB <contact@arccore.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * -------------------------------- Arctic Core ------------------------------*/
//////////////////////////////////////////////////////////

#ifndef INCLUDES_CAN_H_
#define INCLUDES_CAN_H_



#include "ComStack_Types.h"
#include "Std_Types.h"
#include "Mcu_Cfg.h"
/*[SWS_Can_00435] ⌈The Can.h file shall include Can_GeneralTypes.h.*/
#include "Can_GeneralTypes.h"
#include "Can_Cfg.h"





#define CAN_CONTROLLER_ZERO 0U
#define CAN_CONTROLLER_ONE  1U


#define CAN_DISABLE_INTERRUPT_NOT_CALLED     0



#if defined(CFG_PPC)

/* HOH flags */
#define CAN_HOH_FIFO_MASK           (1UL<<0)
//#define CAN_HOH_EOL_MASK            (1<<9)

/* Controller flags */
#define CAN_CTRL_RX_PROCESSING_INTERRUPT        (1UL<<0)
#define CAN_CTRL_RX_PROCESSING_POLLING          0
#define CAN_CTRL_TX_PROCESSING_INTERRUPT        (1UL<<1)
#define CAN_CTRL_TX_PROCESSING_POLLING          0
#define CAN_CTRL_WAKEUP_PROCESSING_INTERRUPT    (1UL<<2)
#define CAN_CTRL_WAKEUP_PROCESSING_POLLING      0
#define CAN_CTRL_BUSOFF_PROCESSING_INTERRUPT    (1UL<<3)
#define CAN_CTRL_BUSOFF_PROCESSING_POLLING      0
#define CAN_CTRL_ACTIVATION                     (1UL<<4)

#define CAN_CTRL_LOOPBACK                       (1UL<<5)
#define CAN_CTRL_FIFO                           (1UL<<6)

#define CAN_CTRL_ERROR_PROCESSING_INTERRUPT    (1UL<<7)
#define CAN_CTRL_ERROR_PROCESSING_POLLING      0


#endif



void Can_Init(Can_ConfigType* Config);
void Can_DeInit(void);
Std_ReturnType Can_SetBaudrate(uint8_t Controller, uint16_t BaudRateConfigID );
Std_ReturnType Can_SetControllerMode( uint8_t Controller,Can_StateTransitionType Transition );
Std_ReturnType Can_GetControllerMode( uint8_t Controller, Can_ControllerStateType* ControllerModePtr);



void Can_EnableControllerInterrupts(uint8_t Controller);
void Can_DisableControllerInterrupts(uint8_t Controller);
void Can_MainFunction_Mode(void);
void  Can_Main_Function_Read(void);


Std_ReturnType Can_Write( Can_HwHandleType HTH, Can_PduType* PduInfo );
void  Can_Main_Function_Write(void);
Std_ReturnType Can_GetControllerErrorState( uint8_t ControllerId, Can_ErrorStateType* ErrorStatePtr);











#endif /* INCLUDES_CAN_H_ */

//////////////////////////////////////////////////////////

#ifndef CAN_H_
#define CAN_H_

#include "Modules.h"

#define CAN_VENDOR_ID			    VENDOR_ID_ARCCORE
#define CAN_MODULE_ID			    MODULE_ID_CAN
#define CAN_AR_MAJOR_VERSION	3
#define CAN_AR_MINOR_VERSION	1
#define CAN_AR_PATCH_VERSION	5

#define CAN_SW_MAJOR_VERSION	1
#define CAN_SW_MINOR_VERSION 	0
#define CAN_SW_PATCH_VERSION	0


/** @name Service id's */
//@{
#define CAN_INIT_SERVICE_ID                         0x00
#define CAN_MAINFUNCTION_WRITE_SERVICE_ID           0x01
#define CAN_INITCONTROLLER_SERVICE_ID               0x02
#define CAN_SETCONTROLLERMODE_SERVICE_ID            0x03
#define CAN_DISABLECONTROLLERINTERRUPTS_SERVICE_ID  0x04
#define CAN_ENABLECONTROLLERINTERRUPTS_SERVICE_ID   0x05
#define CAN_WRITE_SERVICE_ID                        0x06
#define CAN_GETVERSIONINFO_SERVICE_ID               0x07
#define CAN_MAINFUNCTION_READ_SERVICE_ID            0x08
#define CAN_MAINFUNCTION_BUSOFF_SERVICE_ID          0x09
#define CAN_MAINFUNCTION_WAKEUP_SERVICE_ID          0x0a
#define CAN_CBK_CHECKWAKEUP_SERVICE_ID              0x0b
//@}






#endif /*CAN_H_*/
