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
#include "include/Can.h"

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
/*TODO:void CanIf_ControllerModeIndication(0U,Can_ControllerMode [0U])*/

#define CANIF_CODE                           1
#define CANIF_UNINIT                         0

#define CANIF_MODULE_ID                      MODULE_ID_CANIF
#define CANIF_INSTANCE_ID                    0

#define MAX_DATA_SIZE                        8U

/* Det defines */

#define CANIF_E_PARAM_CANID                  10
#define CANIF_E_PARAM_HOH                    12
#define CANIF_E_PARAM_LPDU                   13
#define CANIF_E_PARAM_CONTROLLERID           15
#define CANIF_E_PARAM_WAKEUPSOURCE           16
#define CANIF_E_PARAM_TRCV                   17
#define CANIF_E_PARAM_TRCVMODE               18
#define CANIF_E_PARAM_TRCVWAKEUPMODE         19
#define CANIF_E_PARAM_CTRLMODE               21
#define CANIF_E_PARAM_PDU_MODE               22
#define CANIF_E_PARAM_POINTER                20
#define CANIF_E_UNINIT                       30
#define CANIF_E_INVALID_TXPDUID              50
#define CANIF_E_INVALID_RXPDUID              60
#define CANIF_E_INIT_FAILED                  80



#define CanIfPrivateSoftwareFilterType       MASK
#define CanIfPrivateDataLengthCheck          FALSE


#define CANIF_INIT_ID                        0x01
#define CANIF_INIT_CONTROLLER_ID             0x02
#define CANIF_SET_CONTROLLER_MODE_ID         0x03
#define CANIF_GET_CONTROLLER_MODE_ID         0x04
#define CANIF_GET_CONTROLLER_ERROR_STATE_ID  0x4B
#define CANIF_TRANSMIT_ID                    0x05
#define CANIF_READTXPDUDATA_ID               0x06
#define CANIF_READTXNOTIFSTATUS_ID           0x07
#define CANIF_READRXNOTIFSTATUS_ID           0x08
#define CANIF_SET_PDU_MODE_ID                0x09
#define CANIF_GET_PDU_MODE_ID                0x0A
#define CANIF_CHECKVALIDATION_ID             0x12
#define CANIF_TX_CONFIRMATION_ID             0x13
#define CANIF_RX_INDICATION_ID               0x14



/********************************************************************************************************/
/********************************************************************************************************/
/************************************************** Global Variable *************************************/
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/



/******************************************************************************************************/
LOCAL VAR(uint8_t,AUTOMATIC) canif_cantp_value [] =  {canIf_Cantp_rx};
LOCAL VAR(uint8_t,AUTOMATIC) canif_PduR_Value[]   =   {vcanf};


LOCAL VAR(CanIf_GlobalType ,AUTOMATIC) CanIf_Global;




extern VAR(CanIfRxPduCfg_s ,AUTOMATIC) CanIfRxPduCfg[MAX_NUM_RX_PDU];
extern VAR(CanIfTxPduCfg_s ,AUTOMATIC) CanIfTxPduCfg[MAX_NUM_TX_PDU];



extern VAR(CanIfHthCfg_s ,AUTOMATIC) CanIfHthCfg[MAX_NUM_HTH];
extern VAR(CanIfHrhCfg_s ,AUTOMATIC) CanIfHrhCfg[MAX_NUM_HRH];



/********************************************************************************************************/
/********************************************************************************************************/
/************************************************** Function*********************************************/
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/


/********************************************************************************************************/
/********************************************************************************************************/
/******************************************* CanIf_Init *************************************************/
/********************************************************************************************************/
/********************************************************************************************************/


FUNC(void,CANIF_CODE) CanIf_Init(CONSTP2VAR(CanIf_ConfigType_s,CANIF_CODE,AUTOMATIC) ConfigPtr)
{
    VAR(uint8_t ,AUTOMATIC)  Controller   ;

    /* [SWS_CANIF_00085] The service CanIf_Init() shall initialize the global variablesand data structures of
	the CanIf including flags and buffers.*/
    for ( Controller = 0; Controller <  MAX_NUM_CHANNELS; Controller++ )
    {
        CanIf_Global.channelData[Controller].Controller_Mode = CAN_CS_STOPPED       ;
        CanIf_Global.channelData[Controller].PduMode         = CANIF_OFFLINE        ;
    }

    CanIf_Global.initRun = TRUE;
}

/********************************************************************************************************/
/********************************************************************************************************/
/******************************************* CanIf_DeInit ***********************************************/
/********************************************************************************************************/
/********************************************************************************************************/


/*   NOT SURE IF THIS IS CORRECT   */
FUNC(void,CANIF_CODE) CanIf_DeInit(void)
{
    CanIf_Global.initRun = FALSE;
}


FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode)
{
    VAR(uint8_t,AUTOMATIC) CanDevolpmentErrorType                    = E_OK                          ;
    VAR(Std_ReturnType,AUTOMATIC) ErrorStatus                        = E_OK                          ;
    VAR(CanIf_Channel_t ,AUTOMATIC ) channel_CanIf_SetControllerMode = (CanIf_Channel_t) ControllerId;


    if( CanIf_Global.initRun == FALSE )
    {
        ErrorStatus = E_NOT_OK;
    }
    else
    {
        /*MISRA*/
    }

    if(channel_CanIf_SetControllerMode >= MAX_NUM_CHANNELS)
    {
        /*[SWS_CANIF_00311] If parameter ControllerId of CanIf_SetControllerMode() has an invalid value,
		the CanIf shall report development error code CANIF_E_PARAM_CONTROLLERID to the Det_ReportError
		service of the DET module, when CanIf_SetControllerMode() is called.*/
        ErrorStatus            = E_NOT_OK                   ;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CANIF_E_PARAM_CONTROLLERID ;
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_CONTROLLER_MODE_ID,CANIF_E_PARAM_CONTROLLERID);
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
        ErrorStatus            = E_NOT_OK               ;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CANIF_E_PARAM_CTRLMODE ;
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_CONTROLLER_MODE_ID,CANIF_E_PARAM_CTRLMODE);

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



FUNC(Std_ReturnType,CANIF_CODE) CanIf_GetControllerMode(VAR(uint8_t ,AUTOMATIC) ControllerId, P2VAR(Can_ControllerStateType,AUTOMATIC,) ControllerModePtr)
{
    VAR(Std_ReturnType,AUTOMATIC) ErrorStatus                      = E_OK                          ;
    VAR(CanIf_Channel_t,AUTOMATIC) channel_CanIf_GetControllerMode = (CanIf_Channel_t) ControllerId;

    if( CanIf_Global.initRun == FALSE )
    {
        ErrorStatus = E_NOT_OK;
    }
    else
    {
        /*MISRA*/
    }
    if(channel_CanIf_GetControllerMode >= MAX_NUM_CHANNELS)
    {
        /*[SWS_CANIF_00313] If parameter ControllerId of CanIf_GetControllerMode() has an invalid, the CanIf shall report
		development error code CANIF_E_PARAM_CONTROLLERID to the Det_ReportError service of the DET, when 
		CanIf_GetControllerMode() is called.*/
        ErrorStatus = E_NOT_OK;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_GET_CONTROLLER_MODE_ID,CANIF_E_PARAM_CONTROLLERID);
#endif
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
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_GET_CONTROLLER_MODE_ID,CANIF_E_PARAM_POINTER);
#endif
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


FUNC(Std_ReturnType,CANIF) CanIf_GetControllerErrorState(VAR(uint8_t,AUTOMATIC) ControllerId,P2VAR( Can_ErrorStateType,CANIF_CODE,AUTOMATIC) ErrorStatePtr)
{
    VAR(Std_ReturnType,AUTOMATIC) ErrorStatus = E_OK  ;
    if( CanIf_Global.initRun == FALSE )
    {
        ErrorStatus = E_NOT_OK;
    }
    else
    {
        /*MISRA*/
    }
    if(ControllerId >= MAX_NUM_CHANNELS)
    {
        /*[SWS_CANIF_00898] If parameter ControllerId of CanIf_GetControllerMode() has an invalid, the CanIf shall report
		development error code CANIF_E_PARAM_CONTROLLERID to the Det_ReportError service of the DET, when 
		CanIf_GetControllerMode() is called.*/
        ErrorStatus = E_NOT_OK;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_GET_CONTROLLER_ERROR_STATE_ID,CANIF_E_PARAM_CONTROLLERID);
#endif
    }
    else
    {
        /*MISRA*/
    }
    if(ErrorStatePtr == NULL)
    {
        /* [SWS_CANIF_00899] If parameter ErrorStatePtr of CanIf_GetControllerError
		development error code CANIF_E_PARAM_POINTER to the Det_ReportError service of the DET, when
		CanIf_GetControllerErrorState() is called.*/
        ErrorStatus = E_NOT_OK;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_GET_CONTROLLER_ERROR_STATE_ID,CANIF_E_PARAM_POINTER);
#endif
    }
    else
    {
        /*MISRA*/
    }

    if ( ErrorStatus == E_OK )
    {
        ErrorStatus = Can_GetControllerErrorState( ControllerId, ErrorStatePtr );
    }
    else
    {
        /*MISRA*/
    }
    return ErrorStatus ;
}



FUNC(Std_ReturnType,CANIF) CanIf_Transmit(VAR(PduIdType,AUTOMATIC) TxPduId,  CONSTP2VAR(PduInfoType,CANIF_CODE,AUTOMATIC) PduInfoPtr)
{
    VAR(Std_ReturnType, AUTOMATIC)          toBeReturned = E_OK ;
    VAR(Can_ControllerStateType,AUTOMATIC)  Can_ControllerState ;
    VAR(uint8_t, AUTOMATIC)                 HTH                 ;
    VAR(uint8_t, AUTOMATIC)                 HTHIndex            ;
    VAR(uint8_t, AUTOMATIC)                 ControllerId        ;
    VAR(Can_PduType,AUTOMATIC)              PduInfo             ;

    if(CanIf_Global. initRun == False)
    {
        toBeReturned = E_NOT_OK;
    }
    else
    {/*MISRA*/}

    if ( PduInfoPtr == NULL )
    {
        toBeReturned = E_NOT_OK;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_TRANSMIT_ID,CANIF_E_PARAM_POINTER);
#endif
    }
    else{/*MISRA*/}

    if( TxPduId >= MAX_NUM_TX_PDU )
    {
        toBeReturned = E_NOT_OK;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_TRANSMIT_ID,CANIF_E_INVALID_TXPDUID);
#endif
    }
    else{/*MISRA*/}

    /*	[SWS_CANIF_00317]
     * The service CanIf_Transmit() shall not accept a transmit
     * request, if the controller mode referenced by ControllerId is different to
     * CAN_CS_STARTED and the channel mode at least for the transmit path is not online
     * or offline active
     */
    ControllerId = CanIfHthCfg[CanIfTxPduCfg[TxPduId].CanIfHthCfgRef].CanIfHthCanCtrlIdRef;

    if( Can_GetControllerMode(ControllerId, &Can_ControllerState) == E_OK  && toBeReturned == E_OK)
    {
        if(Can_ControllerState == CAN_CS_STARTED)
        {
            if( CanIf_Global.channelData[ControllerId].PduMode != CANIF_TX_OFFLINE_ACTIVE && CanIf_Global.channelData[ControllerId].PduMode != CANIF_ONLINE)
            {
                toBeReturned = E_NOT_OK;
            }
            else
            {

            }
        }
        else
        {
            toBeReturned = E_NOT_OK;
        }

        if(toBeReturned == E_OK)
        {

            HTHIndex            = CanIfTxPduCfg[TxPduId].CanIfHthCfgRef;
            PduInfo.sdu         = PduInfoPtr->SduDataPtr                 ;
            PduInfo.length      = PduInfoPtr->SduLength                  ;
            PduInfo.swPduHandle = TxPduId                                ;
            PduInfo.id          = CanIfTxPduCfg[TxPduId].CanIfTxPduCanId ;

            toBeReturned = Can_Write( HTHIndex , &PduInfo);
        }
        else
        {/*  MISRA   */}
    }
    else
    {

    }

    return toBeReturned;
}

FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetPduMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR( CanIf_PduModeType, AUTOMATIC) PduModeRequest)
{
    VAR(uint8_t,AUTOMATIC)                  canIfDevError = E_OK;
    VAR(Std_ReturnType,AUTOMATIC)           toBeReturned  = E_OK;
    VAR(Can_ControllerStateType,AUTOMATIC)  Can_ControllerState ;

    if(CanIf_Global.initRun == TRUE)
    {
        if(ControllerId < MAX_NUM_CHANNELS)
        {
            /*[SWS_CANIF_00874] The service CanIf_SetPduMode() shall not accept any request
             *                  and shall return E_NOT_OK, if the controller mode referenced by ControllerId
             *                  is not in state CAN_CS_STARTED
             */
            toBeReturned = Can_GetControllerMode(ControllerId, &Can_ControllerState);
            if(Can_ControllerState == CAN_CS_STARTED && toBeReturned == E_OK)
            {
                if(IS_PDU_MODE(PduModeRequest))
                {
                    CanIf_Global.channelData[ControllerId].PduMode = PduModeRequest;
                }
            }
            else
            {
                toBeReturned = E_NOT_OK;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_PDU_MODE_ID,CANIF_E_PARAM_PDU_MODE);
#endif
            }
        }
        else
        {
            toBeReturned = E_NOT_OK;
        }
    }
    else
    {
        canIfDevError = CANIF_E_PARAM_CONTROLLERID;
        toBeReturned = E_NOT_OK;
    }
    return toBeReturned;
}

FUNC(Std_ReturnType ,CANIF) CanIf_GetPduMode(VAR(uint8_t,AUTOMATIC) ControllerId,P2VAR(CanIf_PduModeType,CANIF_CODE,AUTOMATIC)  PduModePtr)
{
    VAR(Std_ReturnType,AUTOMATIC) toBeRetrurned = E_OK;

    if(CanIf_Global.initRun == TRUE)
    {
        if(ControllerId < MAX_NUM_CHANNELS)
        {
            if(PduModePtr != NULL)
            {
                *PduModePtr = CanIf_Global.channelData[ControllerId].PduMode;
            }
            else
            {
                toBeRetrurned = E_NOT_OK;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_GET_PDU_MODE_ID,CANIF_E_PARAM_POINTER);
#endif
            }
        }
        else
        {
            toBeRetrurned = E_NOT_OK;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_GET_PDU_MODE_ID,CANIF_E_PARAM_PDU_MODE);
#endif
        }
    }
    else
    {
        toBeRetrurned = E_NOT_OK;
    }
    return toBeRetrurned;
}




/*This service indicates a successful reception of a received CAN Rx LPDU
to the CanIf after passing all filters and validation checks.*/

/*[SWS_CANIF_00392]  If configuration parameters CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API (ECUC_CanIf_00608) and
CANIF_RXPDU_READ_NOTIFYSTATUS (ECUC_CanIf_00595) for the Received L-PDU are set to TRUE, and if CanIf_RxIndication() 
is called, the CanIf shall set the notification status for the Received*/

#if CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API == TRUE && CANIF_RXPDU_READ_NOTIFYSTATUS == TRUE
void CanIf_RxIndication(CONSTP2VAR(Can_HwType,CANIF_CODE,AUTOMATIC) Mailbox, CONSTP2VAR(PduInfoType,CANIF_CODE,AUTOMATIC) PduInfoPtr)
{

    /*
     * TOEASKED: Shall we implement software filter if yes what is the algorithm ?
     *
     *
     * */
    VAR(uint8_t,AUTOMATIC)     canIfDevError = E_OK ;
    VAR(uint8_t,AUTOMATIC)     PduIndex      = 0    ;
    VAR(uint8_t,AUTOMATIC)     Controller    = 0    ;
    VAR(uint8_t,AUTOMATIC)     PduMode       = 0    ;
    VAR(PduInfoType,AUTOMATIC) pduInfo              ;

    if(CanIf_Global.initRun == TRUE )
    {
        /*SWS_CANIF_00421]   If CanIf was not initialized before calling CanIf_RxIndication(), CanIf shall not execute
                             Rx indication handling, when CanIf_RxIndication(), is called.*/
        if (Mailbox == NULL || PduInfoPtr == NULL)
        {
            /*SWS_CANIF_00419] d If parameter PduInfoPtr or Mailbox of
          CanIf_RxIndication() has an invalid value, CanIf shall report development error code CANIF_E_PARAM_POINTER to the Det_ReportError 
          service of the DET module, when CanIf_RxIndication() is called*/

            canIfDevError =E_NOT_OK ;
#if CANIF_DEV_ERROR_DETECT == TRUE
            Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_RX_INDICATION_ID,CANIF_E_PARAM_POINTER);
#endif 
        }
        else 
        {
            if ( Mailbox->Ho >= MAX_NUM_HTH )
            {
                /*[SWS_CANIF_00416]  If parameter Mailbox->Hoh of CanIf_RxIndication() has an invalid value,
                                     CanIf shall report development error code CANIF_E_PARAM_HOH to the Det_ReportError service of
                                     the DET module, when CanIf_RxIndication() is called.*/
                canIfDevError =E_NOT_OK ;
#if CANIF_DEV_ERROR_DETECT == TRUE
                Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_RX_INDICATION_ID,CANIF_E_PARAM_HOH);
#endif 
            }
            else 
            {
                /*MISRA*/
            }

            if (PduInfoPtr -> SduLength > MAX_DATA_SIZE )
            {
                /*If CanIf_RxIndication() is called with invalid PduInfoPtr->SduLength, runtime error CANIF_E_INVALID_DATA_LENGTH is reported (see
                [SWS_CANIF_00168]).*/
                canIfDevError =E_NOT_OK ;
#if CANIF_DEV_ERROR_DETECT == TRUE
                Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_RX_INDICATION_ID,CANIF_E_INVALID_DATA_LENGTH);
#endif   
            }
            else 
            {
                /*MISRA*/
            }

        }
    }
    else 
    {
        /*MISRA*/
    }
    if (canIfDevError == E_OK )
    {
        /*[SWS_CANIF_00423]  Configuration of CanIf_RxIndication(): Each Rx L-PDU (see ECUC_CanIf_00249) has to be configured with
    a corresponding receive indication service of an upper layer module (see [SWS_CANIF_00012]) which is called in
    CanIf_RxIndication().*/
        /*[SWS_CANIF_00415]  Within the service CanIf_RxIndication() the CanIf
    routes this indication to the configured upper layer target service(s).*/

        for(PduIndex =0;PduIndex < MAX_NUM_RX_PDU ;PduIndex++)
        {
            if ( Mailbox ->CanId == CanIfRxPduCfg[PduIndex].CanIfRxPduCanId )
            {
                break;
            }
        }
        if ( PduIndex <MAX_NUM_RX_PDU )
        {
            Controller = CanIfHrhCfg[CanIfRxPduCfg[PduIndex].CanIfRxPduHrhIdRef].CanIfHrhCanCtrlIdRef ;
            PduMode    = CanIf_Global.channelData[Controller].PduMode;
            if( ( ( PduMode == CANIF_OFFLINE) || (PduMode == CANIF_TX_OFFLINE) || (PduMode == CANIF_TX_OFFLINE_ACTIVE) ) )
            {
                /*Not Valid pdu mode */
            }
            else 
            {
                switch( CanIfRxPduCfg[PduIndex].CanIfRxPduUserRxIndicationUL)
                {
                case CAN_TP:
                {
                    PduInfoType CanTpRxPdu;
                    CanTpRxPdu.SduLength = PduInfoPtr->SduLength;
                    CanTpRxPdu.SduDataPtr = (uint8_t *)PduInfoPtr->SduDataPtr;
#if 0
                    CanTp_RxIndication( CanIfRxPduCfg[PduIndex].CanIfRxPduId , &CanTpRxPdu );
#endif
                    break;
                }
                case PDUR:
                {
                    pduInfo.SduLength  = PduInfoPtr->SduLength;
                    pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
#if 0
                    PduR_CanIfRxIndication(CanIfRxPduCfg[PduIndex].CanIfRxPduId,&pduInfo);
#endif
                    break;
                }
                }/*Swich End */
            }
        }
        else
        {
            /*[SWS_CANIF_00417]  If parameter Mailbox->CanId of CanIf_RxIndication() has an invalid value,
                CanIf shall report development error code CANIF_E_PARAM_CANID to the Det_ReportError service of the DET
                module, when CanIf_RxIndication() is called. */
            canIfDevError =E_NOT_OK ;
#if CANIF_DEV_ERROR_DETECT == TRUE
            Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_RX_INDICATION_ID,CANIF_E_PARAM_CANID);
#endif
        }
    }
    else
    {
        /*MISRA*/
    }
}
#endif

FUNC(void,CANIF_CODE) CanIf_TxConfirmation(VAR(PduIdType,AUTOMATIC) CanTxPduId)
                                                                                                                                {
    VAR(uint8_t,AUTOMATIC) upperLayer = CanIfTxPduCfg[CanTxPduId].CanIfTxPduUserTxConfirmationUL;
    if(CanIf_Global.initRun == TRUE)
    {
        if(CanTxPduId >= MAX_NUM_TX_PDU)
        {
                   /*  which Tx confirmation should be called  */
                   switch(upperLayer)
                   {
                   case CAN_TP:
                       /*  the first parameter is the pduid in the upperlayer.
                       Since the can only indicates the canif when the data is sent,
                       the CanIf_TxConfirmation will only confirm with E_OK.
                       E_NOT_OK should be sent through the timeout api which is not supported in this driver
                        */
#if 0
                       CanTp_TxConfirmation(CanIfTxPduCfg[CanTxPduId].CanIfTxPduId, E_OK);
#endif
                       break;
                   case PDUR:
#if 0
                       PDUr_TxConfirmation(CanIfTxPduCfg[CanTxPduId].CanIfTxPduId, E_OK);
#endif
                       break;
                   }
            }
            else
            {
#if CANIF_DEV_ERROR_DETECT == TRUE
                Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_TX_CONFIRMATION_ID,CANIF_E_PARAM_LPDU);
#endif
            }
        }
        else
        {
        }

}




