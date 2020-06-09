#include "include/Can.h"
#include "include/CanIf_Cbk.h"
#pragma diag_push
#pragma CHECK_MISRA("none")
#include "include/PortingHW.h"
#include "Timer0A.h"

#pragma RESET_MISRA("all")


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
CanHardwareObject_s CanHardwareObject[NUM_OF_CAN_CONTROLLERS][ MAX_NUM_OF_HO ];

/*[SWS_Can_00401] Several transmit hardware objects (defined by "CanHwObjectCount")
shall be assigned by one HTH to represent one transmit entity to the upper layer.*/

Can_MailBoxLookUpTables_s Can_MailBoxLookUpTables[NUM_OF_CAN_CONTROLLERS][MAX_NUM_OF_MAILBOXES];




/****************************************************************************************************************************/

static void config_transmit_objects(void);
static void Recieve_objects_config (void);
void Can0_InterruptHandler(void);
void Can1_InterruptHandler(void);



static uint8_t state_transition_flag[2]={0U,0U};






/*[SWS_Can_00016] The Can module shall call CanIf_TxConfirmation to indicate a successful transmission.
It shall either called by the TX-interrupt service routine of the corresponding HW resource or inside the
Can_MainFunction_Write  in case of polling mode.*/

static Can_ControllerStateType Can_ControllerMode [NUM_OF_CAN_CONTROLLERS];

static uint8_t CanDevolpmentErrorType;

static uint8_t IntDisableCount[NUM_OF_CAN_CONTROLLERS];

static Can_StateType CanUnitState = CAN_UNINIT ;



/*[SWS_Can_00103] After power-up/reset, the Can module shall be in the state
CAN_UNINIT.*/

/*All global data with read purpose only should be declared constant.[SRS_BSW_309] */

const Mcu_PerClockConfigType McuPerClockConfigData [NUM_OF_CAN_CONTROLLERS]= SYSCTL_PERIPH;
const Can_ConfigType MailboxCfg_MAP = hohMap;
const Can_ConfigType CanContainer= UserCANCfg;




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



#if (CanRxProcessing0==POLLING) || (CanRxProcessing1==POLLING)

void  Can_MainFunction_Read(void)
{
	uint8_t ControllerId, HOIndex;
	
}
#endif


void  Can_MainFunction_Read(void)
{
	uint8_t index6,index7;
	uint16_t POLLING_Register;
	tCANMsgObject received_CANMessage;
	uint8_t rx_MsgData0[8U];
	PduInfoType rx_PduInfo;
	Can_HwType Rx_Mailbox;
	received_CANMessage.pui8MsgData=rx_MsgData0;
/*    while(1)
        {*/

	CAN0_IF1CMSK_R&=((uint32_t)(~(uint32_t)0x80U));
	CAN1_IF1CMSK_R&=((uint32_t)(~(uint32_t)0x80U));

	for(index6=0U; index6< NUM_OF_HOH;index6++)
	{

		if(MailboxCfg_MAP.CanConfigSet.CanHardwareObject[index6].CanObjectType==receive)
		{

			if(MailboxCfg_MAP.CanConfigSet.CanHardwareObject[index6].CanControllerRef==&CanContainer
				.CanConfigSet.CanController[0])
			{
				for(index7=0U;index7<MailboxCfg_MAP.CanConfigSet.CanHardwareObject[index6].CanHwObjectCount;index7++)
				{

					CAN1_IF1CRQ_R=  Message_Confirmation[index6][index7].mail_box;
					POLLING_Register=((uint16_t)((uint16_t)CAN0_IF1MCTL_R));
					POLLING_Register|=0x7FFFU;
					if( POLLING_Register==0xFFFFU)
					{

                            /*[SWS_Can_00279] On L-PDU reception, the Can module shall call the RX indication callback
                             function CanIf_RxIndication with ID ,Hoh, abstract CanIf ControllerId in parameter Mailbox,
                             and the Data Lengthand pointer to the L-SDU buffer in parameter PduInfoPtr.*/

						CANMessageGet(MailboxCfg_MAP.CanConfigSet.CanHardwareObject[index6].
							CanControllerRef->CanControllerBaseAddress,
							Message_Confirmation[index6][index7].mail_box
							, &received_CANMessage, 0U);
/*                                    UARTprintf("Msg ID=0x%08X len=%u data=0x",
                                               received_CANMessage.ui32MsgID, received_CANMessage.ui32MsgLen);*/

						UARTprintf("%02X \n",received_CANMessage.pui8MsgData[0]);

						Rx_Mailbox.hoh=index6 ;
						Rx_Mailbox.id= received_CANMessage.ui32MsgID ;
						Rx_Mailbox.controllerlId =MailboxCfg_MAP.CanConfigSet.CanHardwareObject[index6]
						.CanControllerRef->CanControllerId;
						rx_PduInfo.SduLength  = received_CANMessage.ui32MsgLen;
						rx_PduInfo.SduDataptr = received_CANMessage.pui8MsgData;

						CanIf_RxIndication(&Rx_Mailbox,&rx_PduInfo);
					}
					else{}
				}
		}
		else{
			for(index7=0U;index7<MailboxCfg_MAP.CanConfigSet.CanHardwareObject[index6].CanHwObjectCount;index7++)
			{

				CAN1_IF1CRQ_R=  Message_Confirmation[index6][index7].mail_box;
				POLLING_Register=((uint16_t)((uint16_t)CAN0_IF1MCTL_R));
				POLLING_Register|=0x7FFFU;
				if( POLLING_Register==0xFFFFU)
				{

                                         /*[SWS_Can_00279] On L-PDU reception, the Can module shall call the RX indication callback
                                          function CanIf_RxIndication with ID ,Hoh, abstract CanIf ControllerId in parameter Mailbox,
                                          and the Data Lengthand pointer to the L-SDU buffer in parameter PduInfoPtr.*/
					PduInfoType received_PduInfo;
					CANMessageGet(MailboxCfg_MAP.CanConfigSet.CanHardwareObject[index6].
						CanControllerRef->CanControllerBaseAddress,
						Message_Confirmation[index6][index7].mail_box
						,&received_CANMessage, 0U);
					UARTprintf("Msg ID=0x%08X len=%u data=0x",
						received_CANMessage.ui32MsgID, received_CANMessage.ui32MsgLen);

					UARTprintf("%02X \n",received_CANMessage.pui8MsgData[0]);

					Rx_Mailbox.hoh=index6 ;
					Rx_Mailbox.id= received_CANMessage.ui32MsgID ;
					Rx_Mailbox.controllerlId =MailboxCfg_MAP.CanConfigSet.CanHardwareObject[index6]
					.CanControllerRef->CanControllerId;
					received_PduInfo.SduLength  = received_CANMessage.ui32MsgLen;
					received_PduInfo.SduDataptr = received_CANMessage.pui8MsgData;

                                                         /*CanIf_RxIndication(&Rx_Mailbox,&received_PduInfo);*/
				} else{}
			}
		}
	}else{}
}
 /*  }*/
}
#endif

/*************************************************************************************************************************/
void Can_MainFunction_Mode( void )
{

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
				Register_Check=((uint8_t)(CAN0_CTL_R & CAN0_CTL_R_INIT_BIT));

				if(Can_ControllerMode [controller]== CAN_CS_STARTED)
				{
					if(Register_Check==0U )
					{
                    /*void CanIf_ControllerModeIndication(0U,Can_ControllerMode [0U])*/
						previous_state_0=Can_ControllerMode[controller];
					}
				}
			}
			else if(Can_ControllerMode [controller]== CAN_CS_STOPPED || Can_ControllerMode [controller]== CAN_CS_SLEEP)
			{
				if(Register_Check==1U )
				{
            		/*void CanIf_ControllerModeIndication(0U,Can_ControllerMode [0U])*/
					previous_state_0=Can_ControllerMode[0U];
				}
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
#if (CanBusoffProcessing0==POLLING || CanBusoffProcessing1==POLLING )
void Can_MainFunction_BusOff(void)
{
	uint32_t status;
	while(1)
	{
#if CanBusoffProcessing0==POLLING
		status= CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
		status&=0x80U;
		if(status==CAN_STS_BOFF)
		{
			Can_ControllerMode [0U]=CAN_CS_STOPPED;
                      /*CanIf_ControllerBusOff(0U)*/
		}else{}
#endif
#if CanBusoffProcessing1==POLLING
		status= CANStatusGet(CAN1_BASE, CAN_STS_CONTROL);
		status&=0x80U;
		if(status==CAN_STS_BOFF)
		{
			Can_ControllerMode [1U]=CAN_CS_STOPPED;
                                      /*CanIf_ControllerBusOff(1U)*/
		}else{}
#endif
	}
}
#endif



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
	/*	TI struct to use in data reception	*/
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
	 */

	mailBoxIndex = CANIntStatus(CanController[Controller].CanControllerBaseAddress, CAN_INT_STS_CAUSE);
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
			CanIf_RxIndication(&Rx_Mailbox,&rxPduInfo);


		}
		else if ( CanHardwareObject[Controller][HwObjectIndex].CanObjectType == CAN_OBJECT_TYPE_TRANSMIT )
		{
			Can_MailBoxLookUpTables[Controller][mailBoxIndex].Confirmation = CAN_MSG_CONFIRMED;
		}
	}

}


void Can0_InterruptHandler(void)
{
	CAN_IRQHandler(CAN_CONTROLLER_ZERO);
	
}

void Can1_InterruptHandler(void) 
{
	CAN_IRQHandler(CAN_CONTROLLER_ONE);

}


















































static void Can_ConfigureHardwareObject(void)
{
	uint8_t canControllerRef,canHardwareObjectIndex;
	tCANMsgObject CANMessage                       ;

	for ( uint8_t HO_Index = 0 ; (HO_Index < MAX_NUM_OF_HO) && (CanHardwareObject[canHardwareObjectIndex]) ;HO_Index++ )
	{
		for( canHardwareObjectIndex = 0 ; canHardwareObjectIndex < CanHardwareObject[HO_Index].CanHwObjectCount; canHardwareObjectIndex++ )
		{
			canControllerRef = CanHardwareObject[canHardwareObjectIndex].CanControllerRef                                           ;
	//Can_MailBoxLookUpTables[canControllerRef][canHardwareObjectIndex].HwObject = HO_Index    ;
			if ( CanHardwareObject[canHardwareObjectIndex].CanObjectType == RECEIVE )
			{
				/*Set psMsgObject->ui32MsgID to the full message ID, or a partial mask to use partial ID matching.(TivaWare)*/
	//CANMessage.ui32MsgID     = CanHardwareObject[canHardwareObjectIndex]
				/*Set psMsgObject->ui32MsgLen to the number of bytes in the expected data frame.         (TivaWare)          */
				CANMessage.ui32MsgLen      = MAX_DATA_LENGTH                                                                   ;
				/*psMsgObject->ui32MsgIDMask bits that are used for masking during comparison.                   (TivaWare) */
	//CANMessage.ui32MsgIDMask =
				if ( CanHardwareObject[HO_Index].CanIdType ==  EXTENDED )
				{/* \e psMsgObject->ui32Flags as follows: Set  MSG_OBJ_RX_INT_ENABLE flag to be interrupted when the data frame is received.
														  Set  MSG_OBJ_USE_ID_FILTER flag to enable identifier-based filtering.(TivaWare)*/
					CANMessage.ui32Flags= MSG_OBJ_RX_INT_ENABLE| MSG_OBJ_USE_ID_FILTER|MSG_OBJ_EXTENDED_ID                     ;
			}
			else
			{
				CANMessage.ui32Flags = MSG_OBJ_RX_INT_ENABLE|MSG_OBJ_USE_ID_FILTER                                          ;
			}
			if ( HO_Index < 32)
			{
				/*Configures a message object in the CAN controller.(TivaWare)                                                          */
				CANMessageSet( CanController[canControllerRef].CanControllerBaseAddress,HO_Index,&CANMessage, MSG_OBJ_TYPE_RX )         ;
			}
			else 
			{
				/*Configures a message object in the CAN controller.(TivaWare)                                                          */
				CANMessageSet( CanController[canControllerRef].CanControllerBaseAddress,HO_Index-32,&CANMessage, MSG_OBJ_TYPE_RX )      ;	
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
	uint16_t BaudrateRef                                                                                                                    ;
	tCANBitClkParms Bit_Time_Parameters0; /* structure containing bit time parameters                                                      */
	if(CanUnitState == CAN_UNINIT)
		/*[SWS_Can_00246]  The function Can_Init shall change the module state to CAN_READY, after initialising all controllers inside the HW Unit.*/
	{
		if  ((Can_ControllerMode [CAN_CONTROLLER_ZERO]== CAN_CS_UNINIT) && ( Can_ControllerMode[CAN_CONTROLLER_ONE]== CAN_CS_UNINIT))
		{
			uint8_t canControllerIndex;
			/*[SWS_Can_00245]  The function Can_Init shall initialize all CAN controllers according to their configuration                                             */
			for (canControllerIndex = 0U; canControllerIndex < NUM_OF_CAN_CONTROLLERS ; canControllerIndex++)
			{
				/* Initialize Can controller Registers     (TivaWare)                                                                                                      */
				CANInit( CanController[canControllerIndex].CanControllerBaseAddress )                                                              ;
				/* store the value of Baudrate refrence to simplify the access in the below lines 											*/
				BaudrateRef = CanController[canControllerIndex].CanControllerDefaultBaudrate                                                       ;
				/*Fill Bit_Time_Parameters0 with time parameter from the conifguration parameters 	                													    */
				Bit_Time_Parameters0.ui32SyncPropPhase1Seg= ( CanControllerBaudrateConfig[ BaudrateRef ].CanControllerSeg1   ) +
				( CanControllerBaudrateConfig[ BaudrateRef ].CanControllerPropSeg)                     ;
				Bit_Time_Parameters0.ui32Phase2Seg        =   CanControllerBaudrateConfig[ BaudrateRef ].CanControllerSeg2                         ;
				Bit_Time_Parameters0.ui32SJW              =   CanControllerBaudrateConfig[ BaudrateRef ].CanControllerSyncJumpWidth                ;
/*TOBEASKED: Shall I replace SysCtlClockGet with cnfg parameter ???*/ 
				Bit_Time_Parameters0.ui32QuantumPrescaler =   SysCtlClockGet()/((Bit_Time_Parameters0.ui32SyncPropPhase1Seg +
					Bit_Time_Parameters0.ui32Phase2Seg+1U)*CanControllerBaudrateConfig[ BaudrateRef ].CanControllerBaudRate*1000U);
				/* Configures the CAN controller bit timing      (TivaWare)                                                                                  */
				CANBitTimingSet(CanContainer.CanConfigSet.CanController[canControllerIndex].CanControllerBaseAddress,&Bit_Time_Parameters0)        ;
				/* Disable Can Controller (TivaWare)                                                                                                                          */
				CANDisable(CanController[canControllerIndex].CanControllerBaseAddress)                                                             ;
				/* One-time writable registers that require initialisation directly after reset shall be initialised by the startup code                                       */
				/*This function registers the interrupt handler in the interrupt vector table, and enables CAN interrupts on the interrupt 
controller; specific CAN interrupt sources must be enabled using CANIntEnable().                                                              */
				CANIntRegister(CanController[canControllerIndex].CanControllerBaseAddress,(&Can_InterruptHandler))            ;
				/* only function can_Init can change controller state from uninit to stopped                                                                                     */
				Can_ControllerMode [canControllerIndex]=CAN_CS_STOPPED                                                                              ;
			/*[SWS_Can_00259]  The function Can_Init shall set all CAN controllers in the state STOPPED.                                                                     */
				/*responsible for incrementing disable_interrupt so that can enable_interrupt can execute
			/* after you have configured some/all the can controllers in your module set this general flag                                                                   */
			}/* End Of the for loop */
				CanUnitState = CAN_READY                                                                                                           ;
			/* [SWS_Can_00250] The function Can_Init shall initialize:  static variables, including flags,Common setting for the complete CAN HW unitCAN  					*/
			/* controller specific settings for each CAN controller                                                                                       					*/
			/* [SWS_Can_00246] The function Can_Init shall change the module state to CAN_READY, after initialising all controllers inside the HW Unit.   					*/
			/*        Configure hardware Objects                                                                                                          					*/
				Can_ConfigureHardwareObject()                                                                                                       ;
			/*            initialize the CanInterruptId parameter in the Can_controller struct that shall interrupt masks that uses in Can_EnableControllerInterrupts       */
				CanInterruptId[CAN_CONTROLLER_ZERO].CanInterruptId = INT_CAN0                                                                       ;
				CanInterruptId[CAN_CONTROLLER_ONE ].CanInterruptId = INT_CAN1                                                                       ;
			}
			else
			{
			/*[SWS_Can_00408] ⌈If development error detection for the Can module is enabled: The function Can_Init shall raise the error CAN_E_TRANSITION*/ 
			/*if the CAN controllers are not in state UNINIT*/
				CanDevolpmentErrorType=CAN_E_TRANSITION;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
					/*Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID,CAN_INIT_SID,CAN_E_TRANSITION);*/
#endif
			}
		}
		else
		{
			CanDevolpmentErrorType=CAN_E_TRANSITION;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
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

/*[SWS_Can_00275] ⌈The function Can_Write shall be non-blocking.*/
	Std_ReturnType Can_Write(Can_HwHandleType HTH , Can_PduType* PduInfo)
	{ 

		tCANMsgObject  sCANMessage        ;
		struct Can_objectCheck temp       ;
		uint8_t i                         ;
		uint8_t j                         ;
		uint32_t REGISTER_1               ;
		uint32_t REGISTER_2               ;
		Std_ReturnType ErrorStatus = E_OK ;

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
			CanDevolpmentErrorType = CAN_E_UNINIT ;
			ErrorStatus            = E_NOT_OK     ;
		}
		if ( HTH >= MAX_NUM_OF_HOH  &&  CanHardwareObject[HTH].CanObjectType == RECEIVE )
		{
		/*[SWS_Can_00217] ⌈If development error detection for the Can module is enabled: The function Can_Write
        shall raise the errorCAN_E_PARAM_HANDLE and shall return E_NOT_OK if the parameter Hth is not a
        configured Hardware transmit Handle.*/
			CanDevolpmentErrorType=CAN_E_PARAM_HANDLE;
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

		/* NB:CanTriggertransmitEnable is not supported so it is always = FALSE       */

			CanDevolpmentErrorType=CAN_E_PARAM_POINTER;
			ErrorStatus            = E_NOT_OK     ;
		}
		if(PduInfo->length>8U)
		{
			CanDevolpmentErrorType=CAN_E_PARAM_DATA_LENGTH;
			ErrorStatus            = E_NOT_OK     ;
		/*[SWS_Can_00218] ⌈The function Can_Write shall return E_NOT_OK and if development error detection
                      for the CAN module is enabled shall raise the error CAN_E_PARAM_DATA_LENGTH,
                      If the length is more than 8 byte*/
		}
		if (ErrorStatus == E_OK )
		{ 
//		if(CanHard w ar eObject[HTH].CanHandleType==BASIC)   what is the functionality of CanHandleType
			{
				for(HO_Index =0U ; HO_Index < ( CanHardwareObject[HTH].CanHwObjectCount ) ; HO_Index ++)
				{
				/* Check if the corresponde mailbox is empty or not                           */
					if ( Can_MailBoxLookUpTables[ HTH + HO_Index ].Confirmation == CdONFIRMED )
					{

						Can_MailBoxLookUpTables [ HTH + HO_Index ] .Confirmation = UN_CONFIRMED                ;
						/* Map the mailboc to pdu for the Rxindation in the ISR 				*/
						Can_MailBoxLookUpTables [ HTH + HO_Index ] .PduId        = PduInfo->swPduHandle        ;

						sCANMessage.ui32MsgID     =  PduInfo->id                                        ;
						sCANMessage.ui32MsgLen    =  PduInfo->length                                    ;
						sCANMessage.ui32MsgIDMask =  0U                                                 ;

					/*[SWS_CAN_00502]  If PduInfo->SduLength does not match possible DLC values CanDrv
                                            shall use the next higher valid DLC for transmission with initialisation of unused bytes
                                            to the value of the corresponding CanFdPaddingValue (see ECUC_Can_00485).*/

						if( PduInfo->length < 1U )
						{
							sCANMessage.ui32MsgLen = 1U                                                                    ;
						}
						else
						{
							sCANMessage.ui32MsgLen=PduInfo->length                                                          ;
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
						if ( HO_Index < 32)
						{
						/*Configures a message object in the CAN controller.(TivaWare)                                                          */
							CANMessageSet( CanController[canControllerRef].CanControllerBaseAddress,HO_Index,&sCANMessage, MSG_OBJ_TYPE_RX )         ;
						}
						else 
						{
						/*Configures a message object in the CAN controller.(TivaWare)                                                          */
							CANMessageSet( CanController[canControllerRef].CanControllerBaseAddress,HO_Index-32,&sCANMessage, MSG_OBJ_TYPE_RX )      ;	
						}
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
					return E__OK;
				}
				else 
				{
					return E_NOT_OK;
				}
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
#if (CanTxProcessing0==POLLING) || (CanTxProcessing1==POLLING)
			void  Can_Main_Function_Write(void)
			{
				uint8_t canHardwareIndex,mailBoxIndex;
				uint16_t pollinRegister;
				uint16_t * PtrToReg;

	/*Check the busy flag */
	// while(HWREG(ui32Base + CAN_O_IF1CRQ) & CAN_IF1CRQ_BUSY)
	//    {
	//    }
	/*Clear 7th bit in CANx_IF1CMSK_R */
	/* Transfer the data in the CAN message object specified by the MNUM field in the CANIFnCRQ register into the CANIFn registers.*/
				CAN0_IF1CMSK_R&=((uint32_t)(~0x80U));
				CAN1_IF1CMSK_R&=((uint32_t)(~0x80U));
	/*MISRA violation*/

	/*casting pointer to integral type unavoidable when addressing memory mapped registers
          or other hardware specific features.*/
				for( canHardwareIndex = 0U ; canHardwareIndex < NUM_OF_HOH ; canHardwareIndex++ )
				{
					if(CanHardwareObject[canHardwareIndex].CanObjectType==TRANSMIT)
					{

			//Can_MailBoxLookUpTables
						if(CanHardwareObject[canHardwareIndex].CanControllerRef == CanController[0])
						{
							PtrToReg = (uint16_t )&CAN0_IF1MCTL_R ;
						}
						else 
						{
							PtrToReg = (uint16_t )&CAN1_IF1MCTL_R ;
						}
						for( mailBoxIndex=0U ; mailBoxIndex< CanHardwareObject[canHardwareIndex].CanHwObjectCount; mailBoxIndex++ )
						{
							if(Message_Confirmation[canHardwareIndex][mailBoxIndex].Confirmation==un_confirmed)
							{
					/*A message transfer is started as soon as there is a write of the message object number to the MNUM
										field when the TXRQST bit in the CANIF1MCTL register is set.*/
								CAN0_IF1CRQ_R=Message_Confirmation[canHardwareIndex][mailBoxIndex].mail_box;
					/*MISRA violation*/

					/*Set Transmit Request bit in CANIF1MCTL register */
								pollinRegister=((uint16_t)(*PtrToReg);
									pollinRegister|=0xFEFFU;
					/*MISRA violation*/
					/*casting pointer to integral type unavoidable when addressing memory mapped
                                           registers or other hardware specific features.*/
					/*This message object is not waiting for transmission if this bit is cleared  */
					if(pollinRegister == 0xFEFFU)/*Message_Confirmation[i][j]=un_confirmed*/
									{
						Message_Confirmation[canHardwareIndex][mailBoxIndex].Confirmation=confirmed;/*when it comes to poll again it wouldn't find
                                        it un_confirmed except when another transmission was requested again*/
						/*GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R ^0x04U;*/

						/*test only*/

						/* CanIf_TxConfirmation(Message_Confirmation[i][j].PduId)*/

						/*[SWS_Can_00016] ⌈The Can module shall call CanIf_TxConfirmation to indicate a
                                      successful transmission.It shall either called by the TX-interrupt service routine
                                      of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                      polling mode.*/


						/*7.8 Notification concept*/

						/*CanDrv must store the all in HTHs pending L-PDU Ids in an array
                                        organized perTH to avoid new search of the L-PDU ID for call of
                                       CanIf_TxConfirmation().*/
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
							else 
							{
			/* MISRA   */
			/* Recieve */
							}
	}/*End of For */
}/* End of function */
#endif






