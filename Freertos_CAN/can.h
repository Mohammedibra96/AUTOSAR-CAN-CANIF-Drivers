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


#include<Compiler.h>
#include <ComStack_Types.h>
#include <Std_Types.h>
#include <Mcu_Cfg.h>
/*[SWS_Can_00435] ⌈The Can.h file shall include Can_GeneralTypes.h.*/
#include <Can_GeneralTypes.h>
#include <Can_Cfg.h>





#define CAN_CONTROLLER_ZERO                   (0U)
#define CAN_CONTROLLER_ONE                    (1U)


#define CAN_DISABLE_INTERRUPT_NOT_CALLED     (0U)

#define IS_VALID_HO(HO) ( (HO) < MAX_NUM_OF_HO )

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



FUNC(void,CAN_CODE) Can_Init(P2VAR(Can_ConfigType,CAN_CODE,AUTOMATIC)  Config);
FUNC(void,CAN_CODE) Can_DeInit(void);
FUNC(Std_ReturnType,CAN_CODE) Can_SetBaudrate(VAR(uint8_t,AUTOMATIC) Controller,VAR(uint16_t,AUTOMATIC) BaudRateConfigID );
FUNC(Std_ReturnType,CAN_CODE) Can_SetControllerMode( VAR(uint8_t,AUTOMATIC) Controller, VAR(Can_StateTransitionType,AUTOMATIC) Transition );
FUNC (Std_ReturnType,CAN_CODE) Can_GetControllerMode( VAR(uint8_t,AUTOMATIC) Controller, P2VAR(Can_ControllerStateType,CAN_CODE,AUTOMATIC)  ControllerModePtr);
FUNC(void,CAN_CODE)  Can_EnableControllerInterrupts(VAR(uint8_t,AUTOMATIC) Controller);
FUNC(void,CAN_CODE)  Can_DisableControllerInterrupts(VAR(uint8_t,AUTOMATIC) Controller);
FUNC(void,CAN_CODE) Can_MainFunction_Mode( void );
FUNC (void,CAN_CODE)  Can_MainFunction_Read(void);
FUNC(Std_ReturnType,CAN_CODE) Can_Write(VAR(Can_HwHandleType,AUTOMATIC) HTH ,CONSTP2VAR(Can_PduType,CAN_CODE,AUTOMATIC) PduInfo);
FUNC(void,CAN_CODE)  Can_MainFunction_Write(void);
FUNC(Std_ReturnType,CAN_CODE) Can_GetControllerErrorState( VAR(uint8_t,AUTOMATIC) ControllerId,P2VAR(Can_ErrorStateType,CAN_CODE,AUTOMATIC)  ErrorStatePtr );











#endif /* INCLUDES_CAN_H_ */

//////////////////////////////////////////////////////////

#ifndef CAN_H_
#define CAN_H_

#include <Modules.h>









#endif /*CAN_H_*/
