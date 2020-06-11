#include "include/Can.h"
#include "include/CanIf_Cbk.h"
#pragma diag_push
#pragma CHECK_MISRA("none")
#include "include/PortingHW.h"
#include "Timer0A.h"

#pragma RESET_MISRA("all")



/*	TODO: move all the variable definitions to the can_lcfg.c		*/

/****************************************************** Jemmy & Hema **********************************************************/
/* 
 *
 *This container contains bit timing related configuration parameters of the CAN /
 * controller(s).The Multiplicity is 1..many /
 */
CanControllerBaudrateConfig  CanControllerBaudrateConfig[MAX_NUM_OF_BAUDRATES ];

/*
 * Describes a mask for hardware-based filtering of CAN identifiers./
 * The CAN identifiers of incoming messages are masked with the appropriate/
 * CanFilterMaskValue.Bits holding a 0 mean don't care,/
 * i.e. do not compare the message's identifier in the respective bit position./
 * The mask shall be build by filling with leading 0./
 * The In case of CanIdType EXTENDED or MIXED a 29 bit mask shall
 */
uint32_t CanHwFilterMask[NUM_FILTERS]   ;

 /*
  * This container contains the configuration parameters of the CAN controller(s).
  */
CanController_s CanController[NUM_OF_CAN_CONTROLLERS];
 /*
 * This container contains the configuration (parameters) of CAN Hardware Objects.
 */
CanHardwareObject_s CanHardwareObject[ MAX_NUM_OF_HO ];


/*		this is not part of the lcfg		*/
/*[SWS_Can_00401] Several transmit hardware objects (defined by "CanHwObjectCount")
shall be assigned by one HTH to represent one transmit entity to the upper layer.*/

Can_MailBoxLookUpTables_s Can_MailBoxLookUpTables[NUM_OF_CAN_CONTROLLERS][MAX_NUM_OF_MAILBOXES];




/****************************************************************************************************************************/

static void config_transmit_objects(void);
static void Recieve_objects_config (void);


static uint8_t state_transition_flag[2]={0U,0U};



/*	TODO: change "static" to local	*/


/*[SWS_Can_00016] The Can module shall call CanIf_TxConfirmation to indicate a successful transmission.
It shall either called by the TX-interrupt service routine of the corresponding HW resource or inside the
Can_MainFunction_Write  in case of polling mode.*/

static Can_ControllerStateType Can_ControllerMode [NUM_OF_CAN_CONTROLLERS];


/*	TODO: ENTER & EXIT CRITICAL SECTION	*/
static uint8_t IntDisableCount[NUM_OF_CAN_CONTROLLERS];

static Can_StateType CanUnitState = CAN_UNINIT ;



/*[SWS_Can_00103] After power-up/reset, the Can module shall be in the state
CAN_UNINIT.*/

/*All global data with read purpose only should be declared constant.[SRS_BSW_309] */


/*  SWS_Can_00230: This function performs software triggered state transitions of the CAN controller State machine  
 *                 Which means that this function changes the state of the controller between started, stopped and sleep
 */


Std_ReturnType Can_SetControllerMode( uint8_t Controller, Can_StateTransitionType Transition )
{   
	uint8_t HwObjectIndex, toBeReturned = E_OK;

  /*  validating parameters  */
	if(CanUnitState != CAN_UNINIT)
	{
		if(Controller < NUM_OF_CAN_CONTROLLERS)
		{
      /*   [SWS_Can_00409]  When the function Can_SetControllerMode (CAN_CS_STARTED) is entered and the
                CAN controller is not in state STOPPED it shall detect a invalid state transition*/
			if(((Transition == CAN_T_START) && ( Can_ControllerMode [Controller]==CAN_CS_STOPPED))|| ((Transition == CAN_T_SLEEP) && (Can_ControllerMode [Controller]== CAN_CS_STOPPED)))
			{
        /*  this flag is set here to indicate to Can_Mainfunction_Mode that a change has been made
         *  it is also cleared at the Can_Mainfunction_Mode
         */
				state_transition_flag[Controller]=1;

				switch(Transition)
				{

					case CAN_T_START :
            /*[SWS_Can_00196]The function Can_SetControllerMode shall enable interrupts that are needed in the new state.*/

           /* [SWS_Can_00261] The function Can_SetControllerMode(CAN_CS_STARTED) shall set the hardware registers in a way 
                              that makes the CAN controller participating on the network.*/

            /*[SWS_Can_00204] The Can module shall track all individual enabling and disabling of interrupts in other functions (i.e. Can_SetControllerMode) 
                            , so that the correct interrupt enable state can be restored.*/

            /* [SWS_Can_00425] Enabling of CAN interrupts shall not be executed, when CAN interrupts have been disabled by function Can_DisableControllerInterrupts.*/
					if(IntDisableCount[Controller]==CAN_DISABLE_INTERRUPT_NOT_CALLED)
					{
						CANIntEnable(CanController[Controller].CanControllerBaseAddress, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
						IntEnable(CanController[Controller].CanInterruptId);
						CANEnable(CanController[Controller].CanControllerBaseAddress);
						Int_enable_in_start_mode[Controller]=1U;
					}
            /* if the interrupt has been enabled before hand */
					else
					{
						CANEnable(CanController[Controller].CanControllerBaseAddress);
					}

            /*  Changing the global controller mode  */
					Can_ControllerMode[Controller] = CAN_CS_STARTED;
					break;

					case CAN_T_STOP :

          /* if the interrupt has been disabled before hand */
					if(IntDisableCount[Controller]!= CAN_DISABLE_INTERRUPT_NOT_CALLED)
					{

						CANDisable(CanController[Controller].CanControllerBaseAddress);

					}
          /*  if this is the first time to disabled the interrupt  */
					else
					{
						CANDisable (CanController[Controller].CanControllerBaseAddress);
						CANIntDisable(CanController[Controller].CanControllerBaseAddress, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

					}

         /* [SWS_Can_00263] The function Can_SetControllerMode(CAN_CS_STOPPED) shall set theThis message object is not waiting for transmission.
                            bits inside the CAN hardware such that the CAN controller stops participating on the network.*/
					for(HwObjectIndex = 0U ; HwObjectIndex < NUM_OF_HOH ; HwObjectIndex++)
					{
						if(CanHardwareObject[Controller][HwObjectIndex].CanObjectType==transmit)
						{
              if(Controller == CAN_CONTROLLER_ZERO)   /*  if this is Can0 then use the registers from Can0 (Note That: if you are accessing the registers at the whole code, please use struct and remove this if)  */
							{
               /* Selects one of the 32 message objects in the message RAM for data transfer. The message objects are numbered from 1 to 32. */
								CAN0_IF1CRQ_R =((uint32_t)( Can_MailBoxLookUpTables[HwObjectIndex].mail_box));
              /* This message object is not waiting for transmission.*/
								CAN0_IF1MCTL_R&=((uint32_t)(~0x100U));
								CAN0_IF1CMSK_R &= ((uint32_t)(~0x80U));
							}
							else if( Controller == CAN_CONTROLLER_ONE )
							{
								CAN1_IF1CRQ_R =((uint32_t)( Can_MailBoxLookUpTables[(HwObjectIndex+32)].mail_box));
								CAN1_IF1MCTL_R&=((uint32_t)(~0x100U));
								CAN1_IF1CMSK_R &= ((uint32_t)(~0x80U));
							}
						}
						else
            {/*MISRA*/}
					}
				Can_ControllerMode[Controller] = CAN_CS_STOPPED;
				break;



          /*[SWS_Can_00258] When the CAN hardware does not support sleep mode and is triggered to transition into SLEEP state, 
                            the Can module shall emulate a logical SLEEP state from which it returns only, when it is triggered by software to transition into STOPPED state.*/
          /*[SWS_Can_00404] The CAN hardware shall remain in state STOPPED, while the logical SLEEP state is active.*/
          /*[SWS_Can_00290] If the CAN HW does not support a sleep mode, 
                            the function Can_SetControllerMode(CAN_CS_SLEEP) shall set the CAN controller to the logical sleep mode.*/
          /* [SWS_Can_00197] The function Can_SetControllerMode shall disable interrupts that are not allowed in the new state. */
				case CAN_T_SLEEP :
         /* if the interrupt has been disabled before hand */
				if(IntDisableCount[Controller]>=1U)
				{

					CANDisable(CanController[Controller].CanControllerBaseAddress);

				}
          /*  if this is the first time to disabled the interrupt  */
				else
				{
					CANDisable (CanController[Controller].CanControllerBaseAddress);
					CANIntDisable(CanController[Controller].CanControllerBaseAddress, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
				}

         /* [SWS_Can_00263] The function Can_SetControllerMode(CAN_CS_STOPPED) shall set theThis message object is not waiting for transmission.
                            bits inside the CAN hardware such that the CAN controller stops participating on the network.*/
				Can_MailBoxLookUpTables[Controller][];
				for(HwObjectIndex = 0U ; HwObjectIndex < NUM_OF_HOH ; HwObjectIndex++)
				{
					if(CanHardwareObject[Controller][HwObjectIndex].CanObjectType==transmit)
					{
              if(Controller == CAN_CONTROLLER_ZERO)   /*  if this is Can0 then use the registers from Can0 (Note That: if you are accessing the registers at the whole code, please use struct and remove this if)  */
						{
               /* Selects one of the 32 message objects in the message RAM for data transfer. The message objects are numbered from 1 to 32. */
							CAN0_IF1CRQ_R =((uint32_t)(Can_MailBoxLookUpTables[HwObjectIndex].mail_box));
              /* This message object is not waiting for transmission.*/
							CAN0_IF1MCTL_R&=((uint32_t)(~0x100U));
							CAN0_IF1CMSK_R &= ((uint32_t)(~0x80U));
						}
						else if( Controller == CAN_CONTROLLER_ONE )
						{
							CAN1_IF1CRQ_R =((uint32_t)( Can_MailBoxLookUpTables[(HwObjectIndex+32)].mail_box));
							CAN1_IF1MCTL_R&=((uint32_t)(~0x100U));
							CAN1_IF1CMSK_R &= ((uint32_t)(~0x80U));
						}
					}
					else
            {/*MISRA*/}
				}
			Can_ControllerMode[Controller] = CAN_CS_SLEEP;
			break;
			default:
			break;
		}

    /*   if the input parameter Transition is not valid, then change the error type and return E_NOT_OK indicationg that the operation did not take place   */
		else
		{
			CanDevolpmentErrorType=CAN_E_TRANSITION;
			toBeReturned = E_NOT_OK;

		}
	}
    /*   if the input parameter Controller is not valid, then change the error type and return E_NOT_OK indicationg that the operation did not take place   */
	else
	{
		CanDevolpmentErrorType=CAN_E_PARAM_CONTROLLER;
		toBeReturned = E_NOT_OK;
	}
}

  /*  cna os not initialized  */
else
{
    /*********************    DET SHALL BE REPORTED HERE    **********************/
	toBeReturned = E_NOT_OK;
}

/**********************************************************************************************************************/
/*[SWS_Can_00262] The function Can_SetControllerMode(CAN_CS_STARTED) shall wait for
 limited time until the CAN controller is fully operational. Compare to SWS_Can_00398.*/

/*[SWS_Can_00264] The function Can_SetControllerMode(CAN_CS_STOPPED) shall wait for
a limited time until the CAN controller is really switched off. Compare to SWS_Can_00398.*/

return toBeReturned;
}
}

  /*[SWS_Can_91015] The service Can_GetControllerMode shall return the mode of the requested CAN controller.*/
Std_ReturnType Can_GetControllerMode( uint8_t Controller, Can_ControllerStateType* ControllerModePtr)
{
	uint8_t toBeReturned = E_OK;
  /*  validating inputs  */
	if(Controller < NUM_OF_CAN_CONTROLLERS)
	{
		if(ControllerModePtr!=NULL)
		{
      /*  check that the can has been initialized before  */
			if(CanUnitState == CAN_READY )
			{
				*ControllerModePtr=Can_ControllerMode [Controller];
			}
			else
			{
    /*********************    DET SHALL BE REPORTED HERE    **********************/
				CanDevolpmentErrorType=CAN_E_UNINIT;
				toBeReturned = E_NOT_OK;    
			}
		}
		else
		{
    /*********************    DET SHALL BE REPORTED HERE    **********************/
			CanDevolpmentErrorType=CAN_E_PARAM_POINTER; 
			toBeReturned = E_NOT_OK;    
		}
	}
	else
	{
    /*********************    DET SHALL BE REPORTED HERE    **********************/
		CanDevolpmentErrorType=CAN_E_PARAM_CONTROLLER; 
		toBeReturned = E_NOT_OK;    
	}

	return toBeReturned;

}



#if (CanRxProcessing==POLLING) 

void  Can_MainFunction_Read(void)
{
	uint8_t HOIndex, mailBoxIndex ;
	for ( mailBoxIndex = 0 ; mailBoxIndex < MAX_NUM_OF_HO; mailBoxIndex ++)
	{
		if(CanHardwareObject[HOIndex].CanObjectType == CAN_OBJECT_TYPE_RECEIVE)
		{
			u8 HwObjectIndex = Can_MailBoxLookUpTables[Controller][mailBoxIndex].HwObject;
			if( CanHardwareObject[Controller][HwObjectIndex].CanObjectType == CAN_OBJECT_TYPE_RECEIVE )
			{
				CANMessageGet(CanController[Controller].CanControllerBaseAddress, mailBoxIndex, &CANMsgObject, 0U);

				Rx_Mailbox.Ho = HwObjectIndex;
				Rx_Mailbox.CanId = CANMsgObject.ui32MsgID ;
				Rx_Mailbox.controllerlId = Controller;
            /* [SWS_Can_00060] Data mapping by CAN to memory is defined in a way that the CAN data byte
            which is received first is array element 0 the CAN data byte which is received last is array element 7*/
				rxPduInfo.SduLength  = CANMsgObject.ui32MsgLen;
				rxPduInfo.SduDataPtr = CANMsgObject.pui8MsgData;

			/*******************************	How to give back the pdu id?	*******************************/
				CanIf_RxIndication(&Rx_Mailbox,&rxPduInfo);
			}

		}
	}
	
}
#endif
/*************************************************************************************************************************/
void Can_MainFunction_Mode( void )
{

		//TOBEASKED: EH EL KALAM DA???
    /*[SWS_Can_00369]  The function Can_MainFunction_Mode shall implement the polling
        of CAN status register flags to detect transition of CAN Controller state. Compare to chapter 7.3.2.*/
	static   Can_ControllerStateType previous_state_0=CAN_CS_STOPPED;
	static   Can_ControllerStateType previous_state_1=CAN_CS_STOPPED;

              /*while loop is eliminated for test only*/
	for (u8 controller = 0; controller < MAX_NUM_CONTROLLER; controller++)
	{
		uint8_t Register_Check;

		if(state_transition_flag[controller])
		{
			if(Can_ControllerMode[controller]!=previous_state_0)

			{   
				state_transition_flag[controller]=0U;
				/*	0 Normal operation 	1 Initialization started	*/
				Register_Check=((uint8_t)(CAN0_CTL_R & CAN0_CTL_R_INIT_BIT));

				if(Can_ControllerMode [controller]== CAN_CS_STARTED)
				{
					if(Register_Check==0U )
					{
                    /*void CanIf_ControllerModeIndication(0U,Can_ControllerMode [0U])*/
						previous_state_0=Can_ControllerMode[controller];
					}
					else{}
				}
			else{}
		}
	else if(Can_ControllerMode [controller]== CAN_CS_STOPPED || Can_ControllerMode [controller]== CAN_CS_SLEEP)
	{
		if(Register_Check==1U )
		{
            		/*void CanIf_ControllerModeIndication(0U,Can_ControllerMode [0U])*/
			previous_state_0=Can_ControllerMode[0U];
		}
		else{}
	}
else
{

}
}
else
{

}
}

}


void  Can_DisableControllerInterrupts(uint8_t Controller)
{
	if(CanUnitState!=CAN_UNINIT)
	{
		if(Controller < NUM_OF_CAN_CONTROLLERS)
		{
          /*[SWS_Can_00202] When Can_DisableControllerInterrupts has been called several times,
         Can_EnableControllerInterrupts must be called as many times before the interrupts are re-enabled.*/
        	/*	in other words this counter increases everytime you call the disable function and to re-enable you need to
        		call this function with the same amount.  */

			if( IntDisableCount[Controller] == CAN_DISABLE_INTERRUPT_NOT_CALLED )
			{
				CANIntDisable(CanController[Controller].CanControllerBaseAddress, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
				IntDisable(CanController[Controller].CanInterruptId);
			}
			else
			{

			}
			IntDisableCount[Controller]++;
		}	
		else
		{
			CanDevolpmentErrorType= CAN_E_PARAM_CONTROLLER;
		}

		else
		{
			CanDevolpmentErrorType=CAN_E_UNINIT;
		}
	}
}




void  Can_EnableControllerInterrupts(uint8_t Controller)
{
	if(CanUnitState!=CAN_UNINIT)
	{
		if(Controller < NUM_OF_CAN_CONTROLLERS)
		{

			if( Can_ControllerMode[Controller] == CAN_CS_SLEEP || Can_ControllerMode[Controller] == CAN_CS_STOPPED)
			{

                    /* [SWS_Can_00208] The function Can_EnableControllerInterrupts shall perform no action when
                      Can_DisableControllerInterrupts has not been called before.*/
				if(IntDisableCount[Controller] != CAN_DISABLE_INTERRUPT_NOT_CALLED)

				{

          /*[SWS_Can_00202] When Can_DisableControllerInterrupts has been called several times,
         Can_EnableControllerInterrupts must be called as many times before the interrupts are re-enabled.*/
        	/*	in other words this counter increases everytime you call the disable function and to re-enable you need to
        		call this function with the same amount.  */
					IntDisableCount[Controller]--;

					if( IntDisableCount[Controller] == CAN_DISABLE_INTERRUPT_NOT_CALLED )
					{
                    /* set interrupt enable, status interrupt enable, error interrupt enable to disable interrupts."*/
						CANIntEnable(CanController[Controller].CanControllerBaseAddress, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
						IntEnable(CanController[Controller].CanInterruptId);
					}
					else
					{

					}
				}
				else
				{
				}	
				else
				{
					CanDevolpmentErrorType= CAN_E_PARAM_CONTROLLER;
				}

				else
				{
					CanDevolpmentErrorType=CAN_E_UNINIT;
				}
			}
		}
	}
}

void CAN_IRQHandler(u8 Controller)
{
	uint32_t mailBoxIndex;
	/*	Buffer to recieve data in	*/
	uint8_t rx_MsgData1[8U]={0U};
	/*	TI struct to use in data reception		(Tivaware)*/
	tCANMsgObject CANMsgObject;
	/*	Point at the buffer defined above	*/
	CANMsgObject.pui8MsgData=rx_MsgData1;
	/*	PduInfo is needed here for Canif_RxIndication which uses Pdu	*/
	PduInfoType rxPduInfo;
	Can_HwType Rx_Mailbox;

	/*	
	 *	This function returns a value upon each interrupt.
	 *	If the value of the interrupt register is in the range 1-32, then this indicates the number of the 
	 *	highest priority message object that has an interrupt pending.
	 *	Otherwise, there is an error.
	 *	(tivaware)
	 */

	mailBoxIndex = CANIntStatus(CanController[Controller].CanControllerBaseAddress, CAN_INT_STS_CAUSE);
	/*	TODO: if zero report det	*/
	/*	change those numbers to macros	*/

	if( mailBoxIndex >= 1 && mailBoxIndex <= 32 )
	{
		u8 HwObjectIndex = Can_MailBoxLookUpTables[Controller][mailBoxIndex].HwObject;
		if( CanHardwareObject[Controller][HwObjectIndex].CanObjectType == CAN_OBJECT_TYPE_RECEIVE )
		{
			CANMessageGet(CanController[Controller].CanControllerBaseAddress, mailBoxIndex, &CANMsgObject, 0U);
			
			Rx_Mailbox.Ho = HwObjectIndex;
			Rx_Mailbox.CanId = CANMsgObject.ui32MsgID ;
			Rx_Mailbox.controllerlId = Controller;
            /* [SWS_Can_00060] Data mapping by CAN to memory is defined in a way that the CAN data byte
            which is received first is array element 0 the CAN data byte which is received last is array element 7*/
			rxPduInfo.SduLength  = CANMsgObject.ui32MsgLen;
			rxPduInfo.SduDataPtr = CANMsgObject.pui8MsgData;
			
			/*******************************	How to give back the pdu id?	*******************************/
			/*	not here in the confirmation only	*/
			CanIf_RxIndication(&Rx_Mailbox,&rxPduInfo);


		}
		else if ( CanHardwareObject[Controller][HwObjectIndex].CanObjectType == CAN_OBJECT_TYPE_TRANSMIT )
		{
			/*	TODO: call canif_confirmation	*/
			Can_MailBoxLookUpTables[Controller][mailBoxIndex].Confirmation = CAN_MSG_CONFIRMED;
		}
		else
		{}
}
else{}

}









































#define START_INDEX			0
#define NEW_HO 				1
#define OLD_HO 				0
#define MIN_DATA_SIZE		1

static void Can_ConfigureHardwareObject(void)
{
	
	uint8_t CanControllerId        	 						;
	uint8_t canHardwareObjectIndex 	 						;
	tCANMsgObject CANMessage       	 						;
	uint8_t HO_Index                 						;
	uint8_t MailBoxCounter[NUM_OF_CAN_CONTROLLERS] = {0}	;
	uint8_t StartMailBoxFlag  		 						;
	uint8_t NumberMailBoxes_PerHO						    ;

	/* Loop for all max hardware object in the System */
	for ( HO_Index = START_INDEX ; HO_Index < MAX_NUM_OF_HO;HO_Index++ )
	{
		StartMailBoxFlag = NEW_HO   ;	
		CanControllerId  = CanHardwareObject[canHardwareObjectIndex].CanControllerId                                           ;
		NumberMailBoxes_PerHO = CanHardwareObject[HO_Index].CanHwObjectCount                             ;
		for( canHardwareObjectIndex = START_INDEX ; canHardwareObjectIndex < NumberMailBoxes_PerHO ; canHardwareObjectIndex++ )
		{
			if ( StartMailBoxFlag == NEW_HO )
			{
				CanHardwareObject[HO_Index].CanMailBoxStart = MailBoxCounter[CanControllerId] ;
				StartMailBoxFlag = OLD_HO 																						 ;
			}
			else 
			{
				Can_MailBoxLookUpTables[CanControllerId][canHardwareObjectIndex].HwObject = MailBoxCounter[CanControllerId];
				if ( CanHardwareObject[canHardwareObjectIndex].CanObjectType == RECEIVE )
				{
				/*psMsgObject->ui32MsgIDMask bits that are used for masking during comparison.                   (TivaWare) */
				/*Set psMsgObject->ui32MsgID to the full message ID, or a partial mask to use partial ID matching.(TivaWare)*/
	//CANMessage.ui32MsgID     = CanHardwareObject[canHardwareObjectIndex]
				/*Set psMsgObject->ui32MsgLen to the number of bytes in the expected data frame.         (TivaWare)          */
					CANMessage.ui32MsgLen      = MAX_DATA_LENGTH                                                                   ;
					if ( CanHardwareObject[HO_Index].CanIdType ==  EXTENDED )
				{/* \e psMsgObject->ui32Flags as follows: Set  MSG_OBJ_RX_INT_ENABLE flag to be interrupted when the data frame is received.
														  Set  MSG_OBJ_USE_ID_FILTER flag to enable identifier-based filtering.(TivaWare)*/
						CANMessage.ui32Flags= MSG_OBJ_RX_INT_ENABLE| MSG_OBJ_USE_ID_FILTER|MSG_OBJ_EXTENDED_ID                     ;
				}
				else
				{
					CANMessage.ui32Flags = MSG_OBJ_RX_INT_ENABLE|MSG_OBJ_USE_ID_FILTER                                          ;
				}
				/*Configures a message object in the CAN controller.(TivaWare)                                                          */
				CANMessageSet( CanController[CanControllerId].CanControllerBaseAddress,MailBoxCounter[CanControllerId],&CANMessage, MSG_OBJ_TYPE_RX )         ;
				MailBoxCounter[CanControllerId]++   ;
			}
		}
		else 
		{
				/* MISRA */
		}
	}
}
}

/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_Init*********************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/


void Can_Init(Can_ConfigType* Config)
{
	
	uint16_t 		Baudrate                 		;
	uint8_t  		CanControllerBaseAddress  		;                                                                                                                  ;
	uint8_t  		CanControllerIndex        		;
	tCANBitClkParms Bit_Time_Parameters0	        ; /* structure containing bit time parameters                                                      */
	
	if(CanUnitState == CAN_UNINIT)
		/*[SWS_Can_00246]  The function Can_Init shall change the module state to CAN_READY, after initialising all controllers inside the HW Unit.*/
	{
		if  (( Can_ControllerMode[CAN_CONTROLLER_ZERO] == CAN_CS_UNINIT ) && ( Can_ControllerMode[CAN_CONTROLLER_ONE] == CAN_CS_UNINIT ))
		{
			/*[SWS_Can_00245]  The function Can_Init shall initialize all CAN controllers according to their configuration                                             */
			for (CanControllerIndex = CAN_CONTROLLER_ZERO ; CanControllerIndex < NUM_OF_CAN_CONTROLLERS ; CanControllerIndex++ )
			{
				Baudrate = CanController[CanControllerIndex].CanControllerDefaultBaudrate                                                       ;
				CanControllerBaseAddress = CanController[CanControllerIndex].CanControllerBaseAddress;
				/* Initialize Can controller Registers     (TivaWare)                                                                                                      */
				CANInit( CanControllerBaseAddress )                                                              ;
				/* store the value of Baudrate refrence to simplify the access in the below lines 											*/
				/*Fill Bit_Time_Parameters0 with time parameter from the conifguration parameters 	                													    */
				Bit_Time_Parameters0.ui32SyncPropPhase1Seg= ( CanControllerBaudrateConfig[ Baudrate ].CanControllerSeg1   ) +
				( CanControllerBaudrateConfig[ Baudrate ].CanControllerPropSeg)                     ;
				Bit_Time_Parameters0.ui32Phase2Seg        =   CanControllerBaudrateConfig[ Baudrate ].CanControllerSeg2                         ;
				Bit_Time_Parameters0.ui32SJW              =   CanControllerBaudrateConfig[ Baudrate ].CanControllerSyncJumpWidth                ;
				/*	TODO: add MCU function to get the clock speed (NOTE THAT: you should update this part if you have MCU module)	*/
				Bit_Time_Parameters0.ui32QuantumPrescaler =   Mcu_GetSysCloock()/((Bit_Time_Parameters0.ui32SyncPropPhase1Seg +
					Bit_Time_Parameters0.ui32Phase2Seg+1U) * CanControllerBaudrateConfig[ Baudrate ].CanControllerBaudRate*1000U);
				/* Configures the CAN controller bit timing      (TivaWare)                                                                                  */
				CANBitTimingSet(CanControllerBaseAddress,&Bit_Time_Parameters0)        ;
				/* Disable Can Controller (TivaWare)                                                                                                                          */
			//	CANDisable(CanControllerBaseAddress)                                                             ;
				/*	TODO: ??	*/
				/* One-time writable registers that require initialisation directly after reset shall be initialised by the startup code                                       */
				/*This function registers the interrupt handler in the interrupt vector table, and enables CAN interrupts on the interrupt 
				controller; specific CAN interrupt sources must be enabled using CANIntEnable().                                                              */
				CANIntRegister(CanControllerBaseAddress,(&Can_InterruptHandler))            ;
				/* only function can_Init can change controller state from uninit to stopped                                                                                     */
				Can_ControllerMode [CanControllerIndex]=CAN_CS_STOPPED                                                                              ;
				/*[SWS_Can_00259]  The function Can_Init shall set all CAN controllers in the state STOPPED.                                                                     */
				/*responsible for incrementing disable_interrupt so that can enable_interrupt can execute
				/* after you have configured some/all the can controllers in your module set this general flag                                                                   */

			}/* End Of the for loop */
				CanUnitState = CAN_READY                                                                                                           ;
			/* [SWS_Can_00250] The function Can_Init shall initialize:  static variables, including flags,Common setting for the complete CAN HW unitCAN  					*/
			/* controller specific settings for each CAN controller                                                                                       					*/
			/* [SWS_Can_00246] The function Can_Init shall change the module state to CAN_READY, after initialising all controllers inside the HW Unit.   					*/
			/* Configure hardware Objects                                                                                                          					*/
				Can_ConfigureHardwareObject()                                                                                                       ;
				CanController[CAN_CONTROLLER_ONE ].CanInterruptId = INT_CAN1                                                                       ;
			/*            initialize the CanInterruptId parameter in the Can_controller struct that shall interrupt masks that uses in Can_EnableControllerInterrupts       */
				CanController[CAN_CONTROLLER_ZERO].CanInterruptId = INT_CAN0                                                                       ;
			}
			else
			{
			/*[SWS_Can_00408] ⌈If development error detection for the Can module is enabled: The function Can_Init shall raise the error CAN_E_TRANSITION*/ 
			/*if the CAN controllers are not in state UNINIT*/
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
				CanDevolpmentErrorType = CAN_E_TRANSITION;
			/*Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID,CAN_INIT_SID,CAN_E_TRANSITION);*/
#endif
			}
		}
		else
		{
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
			CanDevolpmentErrorType = CAN_E_TRANSITION;
		/*Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID,CAN_INIT_SID,CAN_E_TRANSITION);*/
#endif
		/* [SWS_Can_00174] If development error detection for the Can module is enabled The function Can_Init shall raise                                                */
		/* the error CAN_E_TRANSITION if the driver is not in state CAN_UNINIT.                                                                                          */
		}
	}

/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_DeInit*******************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/


	void Can_DeInit(void)
	{
    if(CanUnitState == CAN_READY ) /*[SWS_Can_91011]  development error detection for the Can module is enabled:
                                     The function Can_DeInit shall raise the error CAN_E_TRANSITION if the
                                       driver is not in state CAN_READY.(SRS_BSW_00369) */
		{
        /*[SWS_Can_91012] If development error detection for the Can module is enabled: The function Can_DeInit shall
        raise the error CAN_E_TRANSITION if any of the CAN controllers is in state STARTED.*/
      /*  Caveat: Caller of the Can_DeInit function has to be sure no CAN controller is in the state STARTED*/

			if  ((Can_ControllerMode [CAN_CONTROLLER_ZERO]!= CAN_CS_STARTED) && ( Can_ControllerMode[CAN_CONTROLLER_ONE]!= CAN_CS_STARTED))
			{
        CanUnitState = CAN_UNINIT ;  /*   [SWS_Can_ 91009] The function Can_DeInit shall change the module state to
                               CAN_UNINIT before de-initialising all controllers inside the HW unit.*/
				uint8_t canControllerIndex;
				for (canControllerIndex = 0U; canControllerIndex < NUM_OF_CAN_CONTROLLERS ; canControllerIndex++)
				{
					CANDisable(CanController[canControllerIndex].CanControllerBaseAddress)
					Can_ControllerMode [canControllerIndex]= CAN_CS_UNINIT  ;
				}
			}
			else{
				CanDevolpmentErrorType=CAN_E_TRANSITION;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
		/*Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID,CAN_DEINIT_SID,CAN_E_TRANSITION);*/
#endif
			}
		}
		else
		{
			CanDevolpmentErrorType=CAN_E_TRANSITION;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
	/*Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID,CAN_DEINIT_SID,CAN_E_TRANSITION);*/
#endif
		}
	}



/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_Write********************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/

//TOBEASKED:shall we add cnfg parameter that indicates the start of the mailbox in the struct CanHardwareObject_s 
/*[SWS_Can_00275] ⌈The function Can_Write shall be non-blocking.*/
	Std_ReturnType Can_Write(Can_HwHandleType HTH , Can_PduType* PduInfo)
	{ 

		uint8_t 		CanDevolpmentErrorType      ;
		tCANMsgObject   sCANMessage                 ;
		uint32_t 	    REGISTER_1               	;
		uint32_t 	    REGISTER_2               	;
		Std_ReturnType 	ErrorStatus = E_OK          ;
		uint8_t 		MailBoxIndex                ;
		uint8_t 		CanControllerId             ;
		/* [SWS_Can_00100] Several TX hardware objects with unique HTHs may be configured. The
    	CanIf module provides the HTH as parameter of the TX request.*/

		/*[SWS_Can_00276] The function Can_Write shall store the swPduHandle that is given
    	inside the parameter PduInfo until the Can module calls the CanIf_TxConfirmation
    	for this request where the swPduHandle is given as parameter.*/
		/*canTxPDUid_confirmation[pdu_id]=un_sent;*/

		if(CanUnitState==CAN_UNINIT)
		{
			/*[SWS_Can_00216] If development error detection for the Can module is enabled: The function
       		Can_Write shall raise the error CAN_E_UNINIT and shall return E_NOT_OK if the driver is not yet initia_lized.*/

#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
			CanDevolpmentErrorType = CAN_E_UNINIT ;
#endif 
			ErrorStatus            = E_NOT_OK     ;
		}
		if ( HTH >= MAX_NUM_OF_HOH  &&  CanHardwareObject[HTH].CanObjectType == RECEIVE )
		{
			/*[SWS_Can_00217] ⌈If development error detection for the Can module is enabled: The function Can_Write
    	    shall raise the errorCAN_E_PARAM_HANDLE and shall return E_NOT_OK if the parameter Hth is not a
        	configured Hardware transmit Handle.*/
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
			CanDevolpmentErrorType = CAN_E_PARAM_HANDLE;
#endif
			ErrorStatus            = E_NOT_OK     ;

		}
		if(PduInfo==NULL  || PduInfo->sdu==NULL)
		{

			/*([SWS_CAN_00503] ⌈Can_Write() shall accept a null pointer as SDU (Can_PduType.Can_SduPtrType = NULL) 
			if the trigger transmit API is enabled for this hardware object (CanTriggerTransmitEnable = TRUE).*/

			/*[SWS_CAN_00504] If the trigger transmit API is enabled for the hardware object, Can_Write() shall interpret a null pointer as 
			SDU (Can_PduType.Can_SduPtrType = NULL) as request for using the trigger transmit interface. 
			If so and the hardware object is free, Can_Write() shall call CanIf_TriggerTransmit() with the maximum size of the message buffer
			 to acquire the PDU’s data. NOT SUPPORTED */

			/*[SWS_CAN_00219] ⌈If development error detection for CanDrv is enabled: Can_Write()
            shall raise CAN_E_PARAM_POINTER and shall return E_NOT_OK if the parameter PduInfo
            is a null pointer.*/

			/*[SWS_CAN_00505] ⌈If development error detection for CanDrv is enabled: Can_Write()
            shall raise CAN_E_PARAM_POINTER and shall return E_NOT_OK if the trigger transmit
            API is disabled for this hardware object (CanTriggertransmitEnable = FALSE) and
            the SDU pointer inside PduInfo is a null pointer.*/

#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
			CanDevolpmentErrorType=CAN_E_PARAM_POINTER;
#endif
			ErrorStatus            = E_NOT_OK     ;
		}

		
		if(PduInfo->length> MAX_DATA_LENGTH)
		{
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
			CanDevolpmentErrorType=CAN_E_PARAM_DATA_LENGTH;
#endif 
			ErrorStatus            = E_NOT_OK     ;
		/*[SWS_Can_00218] ⌈The function Can_Write shall return E_NOT_OK and if development error detection
                      for the CAN module is enabled shall raise the error CAN_E_PARAM_DATA_LENGTH,
                      If the length is more than 8 byte*/
		}
		if (ErrorStatus == E_OK )
		{ 
			{
				CanControllerId  = CanHardwareObject[canHardwareObjectIndex].CanControllerId ;
				for( MailBoxIndex = CanHardwareObject[HTH].CanMailBoxStart ; MailBoxIndex < ( CanHardwareObject[HTH].CanHwObjectCount ) ; MailBoxIndex ++)
				{
				/* Check if the corresponde mailbox is empty or not                             */
					if ( Can_MailBoxLookUpTables[ CanControllerId ][ MailBoxIndex ].Confirmation == CdONFIRMED )
					{
						/* Set confirmation to the correspond mailbox                           */
						Can_MailBoxLookUpTables [ CanControllerId ][ MailBoxIndex ] .Confirmation = UN_CONFIRMED                ;
						/* Map the mailboc to pdu for the Rxindation in the ISR 				*/
						Can_MailBoxLookUpTables [ CanControllerId ][ MailBoxIndex ] .PduId        = PduInfo->swPduHandle        ;

						sCANMessage.ui32MsgID     =  PduInfo->id                                        ;
						sCANMessage.ui32MsgLen    =  PduInfo->length                                    ;
						sCANMessage.ui32MsgIDMask =  0U                                                 ;

					/*[SWS_CAN_00502]  If PduInfo->SduLength does not match possible DLC values CanDrv
                                            shall use the next higher valid DLC for transmission with initialisation of unused bytes
                                            to the value of the corresponding CanFdPaddingValue (see ECUC_Can_00485).*/

						if( PduInfo->length < MIN_DATA_SIZE )
						{
							sCANMessage.ui32MsgLen   =    MIN_DATA_SIZE                                                                      ;
						}
						else
						{
							sCANMessage.ui32MsgLen  =     PduInfo->length                                                          ;
						}

					/* [SWS_Can_00059] Data mapping by CAN to memory is defined in a way that the
                                              CAN data byte which is sent out first is array element 0, the CAN data byte which
                                              is sent out last is array element 7 or 63 in case of CAN FD.
                                             N.B:Our TIVA c doesn't support FD*/
						canControllerRef = CanHardwareObject[HTH].CanControllerRef                 ;
						if(CanHardwareObject[HTH].CanIdType==EXTENDED)
						{
							sCANMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE|MSG_OBJ_EXTENDED_ID       ;
						}
						else
						{    
							sCANMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE                           ;
						}
						/*	TODO: hema 3aref xD	*/

						/*Configures a message object in the CAN controller.(TivaWare)                                                          */
						CANMessageSet( CanController[canControllerRef].CanControllerBaseAddress, MailBoxIndex ,&sCANMessage, MSG_OBJ_TYPE_RX )         ;
						ErrorStatus            = E_NOT_OK     ;
					}
					else
					{
					ErrorStatus            = CAN_BUSY    ;/*add break here when u clear multi exit points of function*/
					}
				}/*End of for Loop */
				}

		/*[SWS_Can_00011] The Can module shall directly copy the data from the upper
                      layer buffers. It is the responsibility of the upper layer to keep the buffer consistent until return
                      of function call*/
			}
			else
			{
		/* MISRA */
			}
			return ErrorStatus ;
		}



/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_SetBaudrate**************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/


#if CanSetBaudrateApi == TRUE
		Std_ReturnType Can_SetBaudrate(uint8_t Controller,uint16_t BaudRateConfigID )
		{
			uint8_t ErrorStatus = E_OK;
			uint32_t index8;
			tCANBitClkParms Bit_Time_Parameters;
			if(CanUnitState != CAN_READY)
			{
		/*  [SWS_CAN_00492] If development error detection for the Can module is enabled:
       Specification of CAN Driver
        The function Can_SetBaudrate shall raise the error CAN_E_UNINIT and return
       E_NOT_OK if the driver is not yet init_ialized.*/
				CanDevolpmentErrorType=CAN_E_UNINIT;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
		/*Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID,CAN_SET_BAUDRATE,CAN_E_UNINIT);*/
#endif
				ErrorStatus = E_NOT_OK;
			}
			else 
			{
		/* MISRA */
			}
			if(Controller >=(NUM_OF_CAN_CONTROLLERS ))
			{
		/*[SWS_CAN_00494] If development error detection for the Can module is enabled
                 the function Can_SetBaudrate shall raise the error CAN_E_PARAM_CONTROLLER and
                 return E_NOT_OK if the parameter Controller is out of range.*/
				CanDevolpmentErrorType=CAN_E_PARAM_CONTROLLER;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
		/*Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID,CAN_SET_BAUDRATE,CAN_E_PARAM_CONTROLLER);*/
#endif
				ErrorStatus = E_NOT_OK;
			}
			else 
			{
		/* MISRA */
			}
			if(BaudRateConfigID >= MAX_NUM_OF_BAUDRATES )
			{
		/* [SWS_CAN_00493] If development error detection for the Can module is enabled:
                        The function Can_SetBaudrate shall raise the error CAN_E_PARAM_BAUDRATE
                               and return E_NOT_OK if the parameter BaudRateConfigID has an invalid value.*/
				CanDevolpmentErrorType=CAN_E_PARAM_BAUDRATE;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
#endif
				ErrorStatus = E_NOT_OK;
	        }	/*Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID,CAN_SET_BAUDRATE,CAN_E_PARAM_BAUDRATE);*/
				else 
				{
		/* MISRA */
				}
				if ( ErrorStatus == E_OK)

				{
					Bit_Time_Parameters.ui32SyncPropPhase1Seg=CanControllerBaudrateConfig[BaudRateConfigID ].CanControllerSeg1+
					CanControllerBaudrateConfig[BaudRateConfigID ].
					CanControllerPropSeg;
					Bit_Time_Parameters.ui32Phase2Seg=CanControllerBaudrateConfig[BaudRateConfigID ].CanControllerSeg2;
					Bit_Time_Parameters.ui32SJW=CanControllerBaudrateConfig[BaudRateConfigID ].CanControllerSyncJumpWidth;
					Bit_Time_Parameters.ui32QuantumPrescaler=SysCtlClockGet()/((Bit_Time_Parameters.ui32SyncPropPhase1Seg
						+Bit_Time_Parameters.ui32Phase2Seg+1U)*
					CanControllerBaudrateConfig[BaudRateConfigID ].CanControllerBaudRate*1000U);


					CANBitTimingSet(CanController[Controller].CanControllerBaseAddress,&Bit_Time_Parameters);
					if( Can_ControllerMode [Controller]==CAN_CS_STOPPED||Can_ControllerMode [Controller]==CAN_CS_SLEEP)
					{
						CANDisable(CanController[Controller].CanControllerBaseAddress);
					}else
					{
			/*MISRA*/
					}
					ErrorStatus = E_OK;
				}
				else 
				{
					ErrorStatus= E_NOT_OK;
				}
				return ErrorStatus ;
			}
#endif





/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_Main_Function_Write******************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/

/*[SWS_Can_00031] The function Can_MainFunction_Write shall perform the polling of TX confirmation when CanTxProcessingis
set to POLLING or mixed. In case of mixed processing only the hardware objects for which CanHardwareObjectUsesPolling is set
 to TRUE shall be polled (SRS_BSW_00432, SRS_BSW_00373, SRS_SPAL_00157)*/
#if ( CAN_CONTROLLER_CAN_TX_PROCESSING == POLLING )
			void  Can_Main_Function_Write(void)
			{
				uint8_t CanMailBoxIndex,mailBoxIndex ;
				uint16_t pollinRegister              ;
				uint16_t * PtrToReg                  ;

	/*Check the busy flag */
	// while(HWREG(ui32Base + CAN_O_IF1CRQ) & CAN_IF1CRQ_BUSY)
	//    {
	//    }
	/*Clear 7th bit in CANx_IF1CMSK_R */
	/* Transfer the data in the CAN message object specified by the MNUM field in the CANIFnCRQ register into the CANIFn registers.*/
				CAN0_IF1CMSK_R&= ((uint32_t)(~0x80U));
				CAN1_IF1CMSK_R&= ((uint32_t)(~0x80U));
	/* MISRA violation */
	/*casting pointer to integral type unavoidable when addressing memory mapped registers
          or other hardware specific features.*/
				for( CanMailBoxIndex = 0U ; CanMailBoxIndex < MAX_NUM_OF_MAILBOXES ; CanMailBoxIndex++ )
				{
					if( CanHardwareObject[ Can_MailBoxLookUpTables[ CanMailBoxIndex ].HwObject ] .CanObjectType==TRANSMIT )
					{
			//Can_MailBoxLookUpTables
						if( CanHardwareObject[ Can_MailBoxLookUpTables[ CanMailBoxIndex ] ].CanControllerRef == CanController[0] )
						{
							PtrToReg = (uint16_t )&CAN0_IF1MCTL_R ;
						}
						else
						{
							PtrToReg = (uint16_t )&CAN1_IF1MCTL_R ;
						}
						if( Can_MailBoxLookUpTables[ CanMailBoxIndex ].Confirmation==un_confirmed)
						{
				/*A message transfer is started as soon as there is a write of the message object number to the MNUM
										field when the TXRQST bit in the CANIF1MCTL register is set.*/
							if ( CanMailBoxIndex > 32)
							{
								CAN0_IF1CRQ_R = CanMailBoxIndex - 32 ;
							}
							CAN0_IF1CRQ_R = CanMailBoxIndex ;
				/*MISRA violation*/
				/*Set Transmit Request bit in CANIF1MCTL register */
							pollinRegister=((uint16_t)(*PtrToReg);
								pollinRegister|=0xFEFFU;
				/*MISRA violation*/
				/*casting pointer to integral type unavoidable when addressing memory mapped
                                           registers or other hardware specific features.*/
				/*This message object is not waiting for transmission if this bit is cleared  */
//TOBEASKED:shall it be while instade of if ??				
				if( pollinRegister == 0xFEFFU)/*Message_Confirmation[i][j]=un_confirmed*/
								{
					Can_MailBoxLookUpTables[ CanMailBoxIndex ].Confirmation = confirmed;/*when it comes to poll again it wouldn't find
                                        it un_confirmed except when another transmission was requested again*/
					/*GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R ^0x04U;*/
					/*test only*/
					/* CanIf_TxConfirmation(Can_MailBoxLookUpTables[ CanMailBoxIndex ].Confirmation.PduId)*/

					/*[SWS_Can_00016] ⌈The Can module shall call CanIf_TxConfirmation to indicate a
                                      successful transmission.It shall either called by the TX-interrupt service routine
                                      of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                      polling mode.*/


					/*7.8 Notification concept*/

					/*CanDrv must store the all in HTHs pending L-PDU Ids in an array
                                        organized perTH to avoid new search of the L-PDU ID for call of
                                       CanIf_TxConfirmation().*/
								}
							}
							else
							{

							}
						}
						else
						{

						}
	}/*End of For */
					}




















/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_GetControllerErrorState**************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/



/* TOBEASKED : In sws the implementation must be enum  ??????????*/
#define CAN_ERRORSTATE_BUSOFF  BOFF
#define CAN_ERRORSTATE_ACTIVE  EWARN
#define CAN_ERRORSTATE_PASSIVE EPASS



					Std_ReturnType Can_GetControllerErrorState( uint8_t ControllerId, Can_ErrorStateType* ErrorStatePtr )
					{
						Std_ReturnType RetuenValue =  E_OK ;	
						static Can_ErrorStateType  ErrorState[ NUM_OF_CAN_CONTROLLERS];
						uint8_t error_status;
						if(CanUnitState==CAN_UNINIT)
						{
		/*[SWS_Can_91005] ⌈If development error detection for the Can module is enabled: if the module is not yet initialized, 
the function Can_GetControllerErrorState shall raise development error CAN_E_UNINIT and return E_NOT_OK.*/
							CanDevolpmentErrorType=CAN_E_UNINIT;
							RetuenValue = E_NOT_OK;
						}
						if( ControllerId >= NUM_OF_CAN_CONTROLLERS)
						{
							CanDevolpmentErrorType=CAN_E_PARAM_CONTROLLER;
							RetuenValue = E_NOT_OK;
						}
						if( ErrorStatePtr == NULL )
						{
							CanDevolpmentErrorType = CAN_E_PARAM_POINTER ;
							RetuenValue = E_NOT_OK                       ;
						}

						if ( RetuenValue == E_NOT_OK )
						{
		/* Reads one of the controller status registers. (Tivaware)*/
		/* - \b CAN_STATUS_BUS_OFF - controller is in bus-off condition
//TOBEASKED : there are three types of error only that are defines 
//! - \b CAN_STATUS_EWARN - an error counter has reached a limit of at least 96
//! - \b CAN_STATUS_EPASS - CAN controller is in the error passive state
//! - \b CAN_STATUS_RXOK - a message was received successfully (independent of
//!   any message filtering).
//! - \b CAN_STATUS_TXOK - a message was successfully transmitted
//! - \b CAN_STATUS_LEC_MSK - mask of last error code bits (3 bits)
//! - \b CAN_STATUS_LEC_NONE - no error
//! - \b CAN_STATUS_LEC_STUFF - stuffing error detected
//! - \b CAN_STATUS_LEC_FORM - a format error occurred in the fixed format part
//!   of a message
//! - \b CAN_STATUS_LEC_ACK - a transmitted message was not acknowledged
//! - \b CAN_STATUS_LEC_BIT1 - dominant level detected when trying to send in
//!   recessive mode
//! - \b CAN_STATUS_LEC_BIT0 - recessive level detected when trying to send in
//!   dominant mode
//! - \b CAN_STATUS_LEC_CRC - CRC error in received message
//!*/		
							*ErrorStatePtr  = CANStatusGet(CanController[ControllerId].CanControllerBaseAddress, CAN_STS_CONTROL) ;
							error_status& = 0xE0U                                                                                 ;
		RetuenValue = E_OK;/*must be handled with os*/
						}
						else 
						{
		/*MISRA*/
						}
						return RetuenValue ;
					}







