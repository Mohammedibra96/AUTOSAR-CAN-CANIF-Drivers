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



/* Det defines */





#define CANIF_E_PARAM_CANID             10
#define CANIF_E_PARAM_HOH               12
#define CANIF_E_PARAM_LPDU              13
#define CANIF_E_PARAM_CONTROLLERID      15
#define CANIF_E_PARAM_WAKEUPSOURCE      16
#define CANIF_E_PARAM_TRCV              17
#define CANIF_E_PARAM_TRCVMODE          18
#define CANIF_E_PARAM_TRCVWAKEUPMODE    19
#define CANIF_E_PARAM_CTRLMODE          21
#define CANIF_E_PARAM_PDU_MODE          22
#define CANIF_E_PARAM_POINTER           20
#define CANIF_E_UNINIT                  30
#define CANIF_E_INVALID_TXPDUID         50
#define CANIF_E_INVALID_RXPDUID         60
#define CANIF_E_INIT_FAILED             80

#define CanIfPrivateSoftwareFilterType   MASK
#define CanIfPrivateDataLengthCheck      FALSE


#define CAN_GET_CONTROLLER_ERROR_STATE_SID    0x4B


#define CANIF_INIT_ID                0x01
#define CANIF_INIT_CONTROLLER_ID     0x02
#define CANIF_SET_CONTROLLER_MODE_ID 0x03
#define CANIF_GET_CONTROLLER_MODE_ID 0x04
#define CANIF_TRANSMIT_ID            0x05
#define CANIF_READTXPDUDATA_ID       0x06
#define CANIF_READTXNOTIFSTATUS_ID   0x07
#define CANIF_READRXNOTIFSTATUS_ID   0x08
#define CANIF_SETPDUMODE_ID          0x09
#define CANIF_GETPDUMODE_ID          0x0A
#define CANIF_SETDYNAMICTX_ID        0x0C
#define CANIF_SET_TRANSCEIVERMODE_ID     0x0D
#define CANIF_GET_TRANSCEIVERMODE_ID     0x0E
#define CANIF_GET_TRCVMODEREASON_ID      0x0F
#define CANIF_SET_TRANSCEIVERWAKEMODE_ID 0x10
#define CANIF_CHECKWAKEUP_ID          0x11
#define CANIF_CHECKVALIDATION_ID      0x12
#define CANIF_TXCONFIRMATION_ID       0x13
#define CANIF_RXINDICATION_ID         0x14
#define CANIF_CANCELTXCONFIRMATION_ID 0x15
#define CANIF_CONTROLLER_BUSOFF_ID    0x16

#define CANIF_SETWAKEUPEVENT_ID       0x40
#define CANIF_ARCERROR_ID           0x41



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





VAR(CanIf_GlobalType ,AUTOMATIC) CanIf_Global;




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




FUNC(Std_ReturnType,CANIF_CODE) CanIf_GetControllerMode(VAR(uint8_t ,AUTOMATIC) ControllerId, P2VAR(Can_ControllerStateType,AUTOMATIC,) ControllerModePtr)
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
    if(ControllerId >= CANIF_CHANNEL_CNT)
    {
        /*[SWS_CANIF_00898] If parameter ControllerId of CanIf_GetControllerMode() has an invalid, the CanIf shall report
		development error code CANIF_E_PARAM_CONTROLLERID to the Det_ReportError service of the DET, when 
		CanIf_GetControllerMode() is called.*/
        ErrorStatus = E_NOT_OK;
        /*Det_ReportError(CANIF_MODULE_ID,CAN_INSTANCE_ID,CAN_GET_CONTROLLER_ERROR_STATE_SID,CANIF_E_PARAM_CONTROLLERID);*/
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
        /*Det_ReportError(CANIF_MODULE_ID,CAN_INSTANCE_ID,CAN_GET_CONTROLLER_MODE_SID,CANIF_E_PARAM_POINTER);*/
    }	else
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



FUNC(Std_ReturnType,CANIF) CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
                {

                }


FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetPduMode(uint8_t ControllerId, CanIf_PduModeType PduModeRequest)
                {
    VAR(uint8_t,AUTOMATIC)                  canIfDevError = E_OK;
    VAR(Std_ReturnType,AUTOMATIC)           toBeReturned  = E_OK;
    VAR(Can_ControllerStateType,AUTOMATIC)  Can_ControllerState ;


    if(CanIf_Global.initRun == TRUE)
    {
        if(ControllerId < CANIF_CHANNEL_CNT)
        {
            /*[SWS_CANIF_00874] The service CanIf_SetPduMode() shall not accept any request
             *                  and shall return E_NOT_OK, if the controller mode referenced by ControllerId
             *                  is not in state CAN_CS_STARTED
             */
            Can_GetControllerMode(ControllerId, &Can_ControllerState);
            if(Can_ControllerState == CAN_CS_STARTED)
            {
                if(IS_PDU_MODE(PduModeRequest))
                {
                    switch(PduModeRequest)
                    {
                    case CANIF_OFFLINE:
                        CanIf_Global.channelData[ControllerId].PduMode = CANIF_OFFLINE;
                        break;

                    case CANIF_TX_OFFLINE:
                        CanIf_Global.channelData[ControllerId].PduMode = CANIF_TX_OFFLINE;
                        break;

                    case CANIF_TX_OFFLINE_ACTIVE:
                        CanIf_Global.channelData[ControllerId].PduMode = CANIF_TX_OFFLINE_ACTIVE;
                        break;


                    case CANIF_ONLINE:
                        CanIf_Global.channelData[ControllerId].PduMode = CANIF_ONLINE;
                        break;

                    default:
                        break;
                    }
                }
                else
                {
                    toBeReturned = E_NOT_OK;
                    /*   report det CANIF_E_PARAM_PDU_MODE   */
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

    }
    else
    {
        toBeReturned = E_NOT_OK;
    }

    return toBeReturned;
                }


FUNC(Std_ReturnType ,CANIF) CanIf_GetPduMode(uint8_t ControllerId, CanIf_PduModeType* PduModePtr)
                {
    VAR(Std_ReturnType,AUTOMATIC) toRet = E_OK;
    if(CanIf_Global.initRun == TRUE)
    {
        if(ControllerId < CANIF_CHANNEL_CNT)
        {
            if(PduModePtr != NULL)
            {
                *PduModePtr = CanIf_Global.channelData[ControllerId].PduMode;
            }
            else
            {
                toRet = E_NOT_OK;
                /*    report det  CANIF_E_PARAM_POINTER */
            }
        }
        else
        {
            toRet = E_NOT_OK;
            /*    report det  CANIF_E_PARAM_CONTROLLERID */
        }
    }
    else
    {
        toRet = E_NOT_OK;
    }
    return toRet;
                }







FUNC(void,CANIF_CODE) CanIf_RxIndication(const Can_HwType* Mailbox, const PduInfoType *PduInfoPtr)
                {



}

FUNC(void,CANIF_CODE) CanIf_TxConfirmation(PduIdType CanTxPduId)
                {

                }




