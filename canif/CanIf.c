#include "include/CanIf.h"
#include "include/CanIf_Types.h"
#include "include/CanIf_Cfg.h"
#include "include/ComStack_types.h"
#include "include/Std_Types.h"
#include "include/Can_GeneralTypes.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "include/PduR.h"
#include "include/CanIf_CanTp.h"
#include "include/PduR_CanIf.h"


#if defined(USE_PDUR)
#include "PduR.h"
#endif

#if defined(USE_COM)
#include "Com.h"
#endif

/********************************************************************************************************/
/********************************************************************************************************/
/************************************************** MACROS **********************************************/
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/









#define CANIF_CODE                       1

#define CANIF_UNINIT                     0





#define CanIfPrivateSoftwareFilterType   MASK
#define CanIfPrivateDataLengthCheck      FALSE





/********************************************************************************************************/
/********************************************************************************************************/
/************************************************** Global Variable *************************************/
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/

LOCAL VAR( uint8_t ,AUTOMATIC) CANIF_UnitState = CANIF_UNINIT                                         ;







/******************************************************************************************************/
uint8_t canif_cantp_value [] =  {canIf_Cantp_rx};
uint8_t canif_PduR_Value[]   =   {vcanf};
uint8_t canif_Comfrimation_Value[] = {canIf_Comfrimation_Value} ;

CanIf_ConfigType *CanIf_ConfigPtr;

CanIfPrivateCfg *CanIfPrivateCfgInst;

//sint8 RxPdu_Index;

/* typedef struct
{
    Can_ControllerStateType  Controller_Mode;
    CanIf_PduModeType  PduMode;
}CanIf_ChannelPrivateType; //CanIf_ChannelModesType

typedef struct
{
    boolean initRun;
    CanIf_ChannelPrivateType channelData[CANIF_CHANNEL_CNT];
}CanIf_GlobalType;
 */

CanIf_GlobalType CanIf_Global;

/*Array with the size of amount of all the buffers to indicate how much of the buffer is full*/
//uint8_t CanIf_Pdu_BufferIndex[NUM_OF_HTHS];


CanIfInitCfg CanIF_UnInitConfig = {0,0,0,0,0,NULL,NULL,NULL};
//CanIf_ConfigType*   Config_UnInit = {NULL,&CanIF_UnInitConfig,NULL,NULL};

CanIfRxPduCfg *CanIfRxPduCfg_Arr;




/********************************************************************************************************/
/********************************************************************************************************/
/************************************************** Function*********************************************/
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/

#define CAN_INIT_SID  0x01
#define CAN_SET_CONTROLLER_MODE_SID 0x03
#define CAN_GET_CONTROLLER_MODE_SID 0x04


FUNC(void,CANIF_CODE) CanIf_Init(const CanIf_ConfigType* ConfigPtr)
{
	VAR(uint8_t ,AUTOMATIC)  Controller              ;
	/* [SWS_CANIF_00085] The service CanIf_Init() shall initialize the global variablesand data structures of 
	the CanIf including flags and buffers.*/
	for ( Controller = 0; Controller <  CANIF_CHANNEL_CNT; Controller++)
	{
		CanIf_Global.channelData[Controller].Controller_Mode = CAN_CS_STOPPED       ;
		CanIf_Global.channelData[Controller].PduMode         = CANIF_OFFLINE        ;
	}

	CanIf_Global.initRun = TRUE;
}


FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode)
{
	VAR(uint8_t,AUTOMATIC) CanDevolpmentErrorType    = E_OK            ;
	VAR(Std_ReturnType,AUTOMATIC) ErrorStatus        = E_OK            ;
	VAR(CanIf_ChannelIdType ,AUTOMATIC ) channel_CanIf_SetControllerMode = (CanIf_ChannelIdType) ControllerId;
	
	if( CanIf_Global.initRun == FALSE )
	{
		ErrorStatus = E_NOT_OK;
	}
	else
	{
		/*MISRA*/
	}
	if(channel_CanIf_SetControllerMode >= CANIF_CHANNEL_CNT)
	{
		/*[SWS_CANIF_00311] If parameter ControllerId of CanIf_SetControllerMode() has an invalid value, 
		the CanIf shall report development error code CANIF_E_PARAM_CONTROLLERID to the Det_ReportError
		service of the DET module, when CanIf_SetControllerMode() is called.*/
		ErrorStatus = E_NOT_OK                              ;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
		CanDevolpmentErrorType = CANIF_E_PARAM_CONTROLLERID ;
		/*Det_ReportError(CANIF_MODULE_ID,CAN_INSTANCE_ID,CAN_SET_CONTROLLER_MODE_SID,CanDevolpmentErrorType);*/
#endif	 
	}
	else 
	{
		/*MISRA*/
	}
	if ( ControllerMode != CAN_CS_STARTED || ControllerMode != CAN_CS_SLEEP || ControllerMode != CAN_CS_STOPPED)
	{
		/* [SWS_CANIF_00774] If parameter ControllerMode of CanIf_SetControllerMode() has an invalid value (not CAN_CS_STARTED,
		CAN_CS_SLEEP or CAN_CS_STOPPED), the CanIfshall report development error code CANIF_E_PARAM_CTRLMODE 
		to the Det_ReportError service of the DET module, when CanIf_SetControllerMode() is called.*/
		ErrorStatus = E_NOT_OK                              ;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == TRUE
		CanDevolpmentErrorType = CANIF_E_PARAM_CTRLMODE ;
		/*Det_ReportError(CANIF_MODULE_ID,CAN_INSTANCE_ID,CAN_SET_CONTROLLER_MODE_SID,CanDevolpmentErrorType);*/

#endif	 
	}
	else 
	{
		/*MISRA*/
	}
	if ( ErrorStatus == E_OK )
	{
		switch ( ControllerMode ) 
		{
			case CAN_CS_STARTED:
			{
				CanIf_SetPduMode(channel_CanIf_SetControllerMode, CANIF_ONLINE);
				CanIf_Global.channelData[channel_CanIf_SetControllerMode].Controller_Mode = CAN_CS_STARTED;
				/* [SWS_CANIF_00308] The service CanIf_SetControllerMode() shall call 
				Can_SetControllerMode(Controller, Transition) for the requested CAN controller.*/
				ErrorStatus = Can_SetControllerMode(ControllerId, CAN_T_START);
				break;
			}
			case CAN_CS_STOPPED:
			{
				CanIf_SetPduMode(channel_CanIf_SetControllerMode, CANIF_OFFLINE);
				CanIf_Global.channelData[channel_CanIf_SetControllerMode].Controller_Mode = CAN_CS_STOPPED;
				ErrorStatus = Can_SetControllerMode(ControllerId, CAN_T_STOP);
				break;
			}
			case CAN_CS_SLEEP:
			{
				CanIf_SetPduMode(channel_CanIf_SetControllerMode, CANIF_OFFLINE);
				CanIf_Global.channelData[channel_CanIf_SetControllerMode].Controller_Mode = CAN_CS_SLEEP;
				ErrorStatus = Can_SetControllerMode(ControllerId, CAN_T_SLEEP);
				break;
			}
			default:
			break;
		}
	}
	else 
	{
		/*MISRA*/
	}
	return ErrorStatus ;
}




FUNC(Std_ReturnType,CANIF_CODE) CanIf_GetControllerMode(VAR(uint8_t ,AUTOMATIC) ControllerId, Can_ControllerStateType* ControllerModePtr)
{
	VAR(Std_ReturnType,AUTOMATIC) ErrorStatus           = E_OK                              ;
	CanIf_ChannelIdType channel_CanIf_GetControllerMode = (CanIf_ChannelIdType) ControllerId;
	if( CanIf_Global.initRun == FALSE )
	{
		ErrorStatus = E_NOT_OK;
	}
	else 
	{
		/*MISRA*/
	}
	if(channel_CanIf_GetControllerMode >= CANIF_CHANNEL_CNT)
	{
		/*[SWS_CANIF_00313] If parameter ControllerId of CanIf_GetControllerMode() has an invalid, the CanIf shall report 
		development error code CANIF_E_PARAM_CONTROLLERID to the Det_ReportError service of the DET, when 
		CanIf_GetControllerMode() is called.*/
		ErrorStatus = E_NOT_OK;
		/*Det_ReportError(CANIF_MODULE_ID,CAN_INSTANCE_ID,CAN_GET_CONTROLLER_MODE_SID,CANIF_E_PARAM_CONTROLLERID);*/
	}
	else 
	{
		/*MISRA*/
	}
	if(ControllerModePtr == NULL)
	{
		/* [SWS_CANIF_00656] If parameter ControllerModePtr of CanIf_GetControllerMode() has an invalid value, 
		the CanIf shall report development error code CANIF_E_PARAM_POINTER to the Det_ReportError service of
		the DET, when CanIf_GetControllerMode() is called.*/
		ErrorStatus = E_NOT_OK;
		/*Det_ReportError(CANIF_MODULE_ID,CAN_INSTANCE_ID,CAN_GET_CONTROLLER_MODE_SID,CANIF_E_PARAM_POINTER);*/

	}	else 
	{
		/*MISRA*/
	}
	if ( ErrorStatus == E_OK )
	{
		*ControllerModePtr = CanIf_Global.channelData[channel_CanIf_GetControllerMode].Controller_Mode;
	}	else 
	{
		/*MISRA*/
	}
	return ErrorStatus ;
}


FUNC(Std_ReturnType,CANIF) CanIf_GetControllerErrorState(uint8_t ControllerId, Can_ErrorStateType* ErrorStatePtr)
{

	/************************************	To be done	********************************/


	//implement

	/***********************************************************************************/
}



FUNC(Std_ReturnType,CANIF) CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{

	/************************************	To be done	********************************/


	//validate input parameters (make sure that the pdu length is not more than 8 bytes)
	//add det function

	/***********************************************************************************/


	Std_ReturnType toRet = E_OK;
	Can_PduType canPdu;
	const CanIfTxPduCfg * txEntry;
	Can_ControllerStateType csMode = (Can_ControllerStateType) 0;
	CanIf_PduModeType pduMode = (CanIf_PduModeType) 0;
	if(CanIf_Global.initRun == 0){
		return E_NOT_OK;
	}
	if(PduInfoPtr == 0){
		return E_NOT_OK;
	}
	txEntry = CanIf_FindTxPduEntry(TxPduId);
	if (txEntry == 0){
		return E_NOT_OK;
	}
    //waiting to get controller id
	CanIf_ChannelIdType channel_CanIf_Transmit = (CanIf_ChannelIdType) txEntry->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->CanIfCtrlId;
	if (CanIf_GetControllerMode(channel_CanIf_Transmit, &csMode) == E_NOT_OK){
		return E_NOT_OK;
	}
	if (csMode != CAN_CS_STARTED){
		return E_NOT_OK;
	}
	if (CanIf_GetPduMode(channel_CanIf_Transmit, &pduMode) == E_NOT_OK){
		return E_NOT_OK;
	}
	if (pduMode != CANIF_ONLINE){
		return E_NOT_OK;
	}
	canPdu.id = txEntry->CanIfTxPduCanId;
	canPdu.length = PduInfoPtr->SduLength;
	canPdu.swPduHandle = TxPduId;
	canPdu.sdu = PduInfoPtr->SduDataPtr;
    //waiting to know who to get HTH id
	toRet= Can_Write(txEntry->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthIdSymRef->CanObjectId,&canPdu);
	return toRet;
}


FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetPduMode(uint8_t ControllerId, CanIf_PduModeType PduModeRequest)
{

	/************************************	To be done	********************************


	//	Validate input parameters
	//	Add det function
	//	The service CanIf_SetPduMode() shall not accept any request
		and shall return E_NOT_OK, if the controller mode referenced by ControllerId
		is not in state CAN_CS_STARTED.


	***********************************************************************************/
	Std_ReturnType toRet;
	CanIf_ChannelIdType channel_CanIf_SetPduMode = (CanIf_ChannelIdType) ControllerId;
	if(CanIf_Global.initRun == 0){
		return E_NOT_OK;
	}
	if(channel_CanIf_SetPduMode >= CANIF_CHANNEL_CNT){
		return E_NOT_OK;
	}
	switch(PduModeRequest){
		case CANIF_OFFLINE:
		CanIf_Global.channelData[channel_CanIf_SetPduMode].PduMode = CANIF_OFFLINE;
		break;
		case CANIF_TX_OFFLINE:
		CanIf_Global.channelData[channel_CanIf_SetPduMode].PduMode = CANIF_TX_OFFLINE;
		break;
        //CANIF_TX_OFFLINE_ACTIVE transmit confirmation not supported
		case CANIF_ONLINE:
		CanIf_Global.channelData[channel_CanIf_SetPduMode].PduMode = CANIF_ONLINE;
		break;
		default:
		break;
	}
	toRet = E_OK;
	return toRet;
}

FUNC(Std_ReturnType ,CANIF) CanIf_GetPduMode(uint8_t ControllerId, CanIf_PduModeType* PduModePtr)
{
	Std_ReturnType toRet;
	CanIf_ChannelIdType CanIf_GetPduMode_local = (CanIf_ChannelIdType)ControllerId;
	if(CanIf_Global.initRun == 0){
		return E_NOT_OK;
	}
	if(CanIf_GetPduMode_local >= CANIF_CHANNEL_CNT){
		return E_NOT_OK;
	}

	*PduModePtr = CanIf_Global.channelData[CanIf_GetPduMode_local].PduMode;

	toRet = E_OK;
	return toRet;
}

const CanIfTxPduCfg * CanIf_FindTxPduEntry(PduIdType TxPduId)
{
	if(TxPduId >= (PduIdType) CanIf_ConfigPtr->InitConfig->CanIfMaxTxPduCfg ){
		return (CanIfTxPduCfg *) NULL;
	}
	uint32 index,i ;
	for (i=0;i<CanIf_ConfigPtr->InitConfig->CanIfMaxTxPduCfg;i++){
		if (TxPduId == CanIf_ConfigPtr->InitConfig->CanIfTxPduConfigPtr->CanIfTxPduId ){
            index = i;                      /**********************   can the index be the pdu id      **********************/
			break;
		}
	}
	return &CanIf_ConfigPtr->InitConfig->CanIfTxPduConfigPtr[index];
}



CanIf_ChannelIdType CanIf_FindHrhChannel (Can_HRHType HRH)
{
	CanIfHrhCfg* HRHCfg;
	sint8 HRHCfg_Counter = 0;
	CanIf_ChannelIdType To_Return;
	uint8_t check = 0;


	HRHCfg = CanIf_ConfigPtr->InitConfig->CanIfHohConfigPtr->CanIfHrhCfgPrt;
	uint8_t len = sizeof(HRHCfg)/sizeof(HRHCfg[0]);

	do
	{
		if((HRHCfg[HRHCfg_Counter].CanIfHrhIdSymRef->HOH) == HRH)
		{
			check = 1;
			To_Return = HRHCfg[HRHCfg_Counter].CanIfHrhCanCtrlIdRef->CanIfCtrlId;
            //RxPdu_Index = HRHCfg_Counter;

		}
		HRHCfg_Counter++;

	}while((check==(uint8_t) 0) && HRHCfg_Counter<len);

	return To_Return;
}


FUNC(sint8_t,CANIF) CanIf_FindRxPduEntry(const Can_HwType* Mailbox)
{
	uint8_t i ;
	sint8 to_return = -1;
	for(i=0;i< CanIf_ConfigPtr->InitConfig->CanIfMaxRxPduCfg;i++)
	{
		if (Mailbox->id == CanIf_ConfigPtr->InitConfig->CanIfRxPduConfigPtr[i].CanIfRxPduCanId){
			to_return = i;
			break;
		}
	}
	return to_return;
}


CanIfRxPduUserRxConfirmationUL Get_User_RxIndication_Name(sint8 i)
{
	CanIfRxPduUserRxConfirmationUL to_return = CanIf_ConfigPtr->InitConfig->CanIfRxPduConfigPtr[i].CanIfRxPduUserRxIndicationUL_i;
	return to_return;
}


FUNC(void , CANIF) Get_User_RxIndication(CanIfRxPduUserRxConfirmationUL user,uint16 i)
{
	CanIfRxPduCfg_Arr = CanIf_ConfigPtr->InitConfig->CanIfRxPduConfigPtr;

	if (user == PDUR)
	{
		CanIfRxPduCfg_Arr[i].CanIfRxPduUserRxIndicationName = USE_PDUR;
	}

	else if (user == CAN_TP)
	{
		CanIfRxPduCfg_Arr[i].CanIfRxPduUserRxIndicationName = USE_TP;
	}

    //return &CanIfRxPduCfg_Arr[i]->CanIfRxPduUserRxIndicationName;
}



FUNC(void,CANIF_CODE) CanIf_RxIndication(const Can_HwType* Mailbox, const PduInfoType *PduInfoPtr)
{

	/************************************	To be done	********************************/


	//validate input parameters
	//add det function

	/***********************************************************************************/
	
	uint8_t cont = 0;
	uint8_t check_t = 0;
	CanIf_PduModeType mode_CanIf_RxIndication = (CanIf_PduModeType) 0;
	uint8_t index;

	uint8_t CanIfRxPduCfg_index = 0;
	CanIf_ChannelIdType channel_CanIf_RxIndication = (CanIf_ChannelIdType) CanIf_FindHrhChannel(Mailbox->hoh);

	sint8 RxPdu_Index = CanIf_FindRxPduEntry(Mailbox);

	CanIfPrivateCfg *CanIfPrivateCfgArr = &CanIfPrivateCfgInst;
	CanIfRxPduCfg* canifArr = CanIf_ConfigPtr->InitConfig->CanIfRxPduConfigPtr;

    //CanIfRxPduCfg_index = CanIf_FindRxPduCfgIndex(Mailbox->controllerlId);

	if ((signed)channel_CanIf_RxIndication == -1)
	{
		check_t =(uint8_t)1;
	}

	while (check_t==(uint8_t)0)
	{
		if((CanIf_GetPduMode(channel_CanIf_RxIndication,&mode_CanIf_RxIndication))==E_OK)
		{
			if(((mode_CanIf_RxIndication == CANIF_OFFLINE) || (mode_CanIf_RxIndication == CANIF_TX_OFFLINE) ||
				(mode_CanIf_RxIndication == CANIF_TX_OFFLINE_ACTIVE)))
			{
				{
					check_t =(uint8_t)1;
				}
			}
		}
		for (index =(uint16) 0;(  (check_t==(uint8_t)0) && (index < (uint16)CanIf_ConfigPtr->InitConfig->CanIfMaxRxPduCfg) ); index++)
		{
			if(((&canifArr[index])->CanIfRxPduHrhIdRef->CanIfHrhIdSymRef->CanHandleType) == BASIC)

			{
               //if((&CanIfPrivateCfgInst[index])->CanIfPrivateSoftwareFilterType == MASKTYPE)
				if(CanIfPrivateSoftwareFilterType == MASK)
				{
					if ((Mailbox->id & (&canifArr[index])->CanIfRxPduCanIdMask ) ==
						( (&canifArr[index])->CanIfRxPduCanId & (&canifArr[index])->CanIfRxPduCanIdMask))
					{
						cont = (uint8_t) 0;
					}
					else
					{
						cont=(uint8_t)1;
					}
				}


			}
			if( cont == 0)
			{

   //if((&CanIfPrivateCfgInst[index])->CanIfPrivateDataLengthCheck == TRUE)
				if(CanIfPrivateDataLengthCheck  == TRUE)
            {//stated in the SWS
            	if (PduInfoPtr->SduLength < (&canifArr[RxPdu_Index])->CanIfRxPduDataLength)
            	{

            		check_t = (uint8_t) 1;
            	}
            }


            CanIfRxPduUserRxConfirmationUL USER_NAME = Get_User_RxIndication_Name(RxPdu_Index);
                //Get_User_RxIndication(canifArr[RxPdu_Index].CanIfRxPduUserRxIndicationUL_i, RxPdu_Index);


            switch(USER_NAME)
            {
            	case CAN_TP:
            	{
            		PduInfoType CanTpRxPdu;
            		CanTpRxPdu.SduLength = PduInfoPtr->SduLength;
            		CanTpRxPdu.SduDataPtr = (uint8_t *)PduInfoPtr->SduDataPtr;

                    /*if(*(CanTpRxPdu.SduDataPtr)==2)
                    {

                    }*/

                    // printf("WESLNAAAAAAAAAAAAAAA ,, DATA %c \n LENGTH %d",*(CanTpRxPdu.SduDataPtr),CanTpRxPdu.SduLength);

            		CanTp_RxIndication(canif_cantp_value[RxPdu_Index],&CanTpRxPdu);

            		check_t = 1;
            		break;
            	}

            	case PDUR:
            	{

            		PduInfoType pduInfo;
            		pduInfo.SduLength = PduInfoPtr->SduLength;
            		pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
            		uint8_t arr[8] = pduInfo.SduDataPtr;

            		PduR_CanIfRxIndication(canif_PduR_Value[RxPdu_Index],&pduInfo);

            		check_t = 1;
            		break;
            	}

            }


        }
    }

}
check_t = (uint8_t)1;
}

CANIF(void,CANIF_CODE) CanIf_TxConfirmation(PduIdType CanTxPduId)
{

	/************************************	To be done	********************************/


	//validate input parameters
	//add det function

	/***********************************************************************************/
	const CanIfTxPduCfg * txEntry;
	txEntry = CanIf_FindTxPduEntry(CanTxPduId);
	uint32 index,i ;
	for (i=0;i<CanIf_ConfigPtr->InitConfig->CanIfMaxTxPduCfg;i++){
		if (CanTxPduId == CanIf_ConfigPtr->InitConfig->CanIfTxPduConfigPtr->CanIfTxPduId ){
			index = i;
			break;
		}
	}

	switch(txEntry->CanIfTxPduUserTxConfirmationUL_i)
	{
		case CAN_TP:

		CanTp_TxConfirmation(canif_Comfrimation_Value[index], E_OK);

		break;
		case PDUR:

		PDUr_TxConfirmation(canif_Comfrimation_Value[index], E_OK);

		break;
		default:

		break;
	}
}




