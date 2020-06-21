#include <Can.h>
#include <Can_GeneralTypes.h>
#include <CanIf.h>
#include <CanIf_CanTp.h>
#include <CanIf_Cfg.h>
#include <CanIf_Types.h>
#include <Det.h>
#include <PduR.h>
#include <PduR_CanIf.h>
#include <Std_Types.h>
#include "ComStack_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "CanIf_Det.h"
#include"CanIf_lcfg.h"

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
/*CANIF_CODE this macro for Can code memory section  */
#define CANIF_CODE                           1

#define MAX_DATA_SIZE_FD                     (uint8_t)64
#define MAX_DATA_SIZE_CLASSIC               (uint8_t)8
#define IS_NOT_VALID_DATA_LENGTH(Datalength,FrameType)  ( ( ( ( (FrameType) == STANDARD_FD_CAN) || ((FrameType) == EXTENDED_FD_CAN)) && ((Datalength) > MAX_DATA_SIZE_FD ))\
        || ( ( (FrameType) == EXTENDED_CAN    || (FrameType) == STANDARD_CAN )    && (Datalength) > MAX_DATA_SIZE_CLASSIC) )

#define IS_VALID_CONTROLLER_ID(Controller) ((Controller) < MAX_CANIF_CTRL )

#define IS_NOT_VALID_CONTROLLER_MODE(ControllerMode) (((ControllerMode) != CAN_CS_STARTED) || ((ControllerMode) != CAN_CS_SLEEP) || ((ControllerMode) != CAN_CS_STOPPED))
#define IS_NOT_PDU_TRANSMIT_MODE() ( (CanIf_Global.channelData[CanifcfgIndex].PduMode != CANIF_TX_OFFLINE_ACTIVE) && (CanIf_Global.channelData[CanifcfgIndex].PduMode != CANIF_ONLINE))
#define IS_VALID_HRH(HTH) ((HTH) < MAX_NUM_HRH)
#define IS_VALID_TX_PDU_ID(TxPduId) ( (TxPduId) < MAX_NUM_TX_PDU )
#define IS_VALID_RX_PDU(PduIndex) ((PduIndex) < MAX_NUM_RX_PDU)

/********************************************************************************************************/
/********************************************************************************************************/
/************************************************** Global Variable *************************************/
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/
/*CanIf_Global contains the Pdu mode, controller mode and the driver status  */
LOCAL VAR(CanIf_GlobalType ,AUTOMATIC) CanIf_Global;


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
    VAR(uint8_t ,AUTOMATIC)  Controller = CANIF_CHANNEL_1  ;
    /* [SWS_CANIF_00085] The service CanIf_Init() shall initialize the global variables and data structures of
	the CanIf including flags and buffers*/
    for (  ; Controller <  (uint8_t)MAX_CANIF_CTRL; Controller++ )
    {
        CanIf_Global.channelData[Controller].Controller_Mode = CAN_CS_STOPPED       ;
        CanIf_Global.channelData[Controller].PduMode         = CANIF_OFFLINE        ;
    }
    CanIf_Global.initRun = (uint8_t)TRUE;
                                                                                                                                                                                                                                                                                                                                                                                                }

/********************************************************************************************************/
/********************************************************************************************************/
/******************************************* CanIf_DeInit ***********************************************/
/********************************************************************************************************/
/********************************************************************************************************/


FUNC(void,CANIF_CODE) CanIf_DeInit(void)
                                                                                                                                                                                                                                                                                                                                                                                                {
    CanIf_Global.initRun = (uint8_t)FALSE;
                                                                                                                                                                                                                                                                                                                                                                                                }


FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode)
                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(Std_ReturnType,AUTOMATIC) ErrorStatus                        = E_OK                          ;
    VAR(uint8_t,AUTOMATIC) CanControllerId                         = 0                             ;

    if( CanIf_Global.initRun == (uint8_t)FALSE )
    {
        ErrorStatus = E_NOT_OK;
    }
    else
    {
        /*MISRA*/
    }
    if( IS_VALID_CONTROLLER_ID(ControllerId) == FALSE )
    {
        ErrorStatus            = E_NOT_OK                   ;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_CONTROLLER_MODE_ID,CANIF_E_PARAM_CONTROLLERID);
#endif	 
        /*[SWS_CANIF_00311] If parameter ControllerId of CanIf_SetControllerMode() has an invalid value,
		the CanIf shall report development error code CANIF_E_PARAM_CONTROLLERID to the Det_ReportError
		service of the DET module, when CanIf_SetControllerMode() is called.*/
    }
    else
    {
        /*MISRA*/
    }
    if ( IS_NOT_VALID_CONTROLLER_MODE(ControllerMode) == (uint8_t)TRUE )
    {
        ErrorStatus            = E_NOT_OK               ;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_CONTROLLER_MODE_ID,CANIF_E_PARAM_CTRLMODE);
#endif	 
        /* [SWS_CANIF_00774] If parameter ControllerMode of CanIf_SetControllerMode() has an invalid value (not CAN_CS_STARTED,
		CAN_CS_SLEEP or CAN_CS_STOPPED), the CanIfshall report development error code CANIF_E_PARAM_CTRLMODE
		to the Det_ReportError service of the DET module, when CanIf_SetControllerMode() is called.*/
    }
    else
    {
        /*MISRA*/
    }
    if ( ErrorStatus == E_OK )
    {

        /* [SWS_CANIF_00308] The service CanIf_SetControllerMode() shall call
				Can_SetControllerMode(Controller, Transition) for the requested CAN controller.*/
        CanControllerId = CanIfCtrlCfg[ControllerId].CanIfCtrlCanCtrlRef ;
        ErrorStatus = Can_SetControllerMode(CanControllerId, ControllerMode);
        if (ErrorStatus == E_OK )
        {
            CanIf_Global.channelData[ControllerId].Controller_Mode = ControllerMode;
        }
        else
        {
            /*MISRA*/
        }
    }
    return ErrorStatus ;
                                                                                                                                                                                                                                                                                                                                                                                                }



FUNC(Std_ReturnType,CANIF_CODE) CanIf_GetControllerMode(VAR(uint8_t ,AUTOMATIC) ControllerId, P2VAR(Can_ControllerStateType,CANIF_CODE,AUTOMATIC) ControllerModePtr)
                                                                                                                                                                                                                 {
    VAR(Std_ReturnType,AUTOMATIC) ErrorStatus                      = E_OK                          ;
    VAR(uint8_t,AUTOMATIC) CanControllerId                         = 0                             ;

    if( CanIf_Global.initRun == (uint8_t)FALSE )
    {
        ErrorStatus = E_NOT_OK;
    }
    else
    {
        /*MISRA*/
    }
    if( IS_VALID_CONTROLLER_ID(ControllerId)== FALSE )
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

    if(ControllerModePtr == NULL_PTR)
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
        CanControllerId = CanIfCtrlCfg[ControllerId].CanIfCtrlCanCtrlRef ;
        ErrorStatus = Can_GetControllerMode(CanControllerId, ControllerModePtr);

    }	else
    {
        /*MISRA*/
    }
    return ErrorStatus ;
                                                                                                                                                                                                                 }


FUNC(Std_ReturnType,CANIF) CanIf_GetControllerErrorState(VAR(uint8_t,AUTOMATIC) ControllerId,P2VAR( Can_ErrorStateType,CANIF_CODE,AUTOMATIC) ErrorStatePtr)
                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(Std_ReturnType,AUTOMATIC) ErrorStatus     = E_OK  ;
    VAR(Std_ReturnType,AUTOMATIC) CanControllerId = E_OK  ;

    if( CanIf_Global.initRun == (uint8_t)FALSE )
    {
        ErrorStatus = E_NOT_OK;
    }
    else
    {
        /*MISRA*/
    }
    if(IS_VALID_CONTROLLER_ID(ControllerId) == FALSE )
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
    if(ErrorStatePtr == NULL_PTR)
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
        CanControllerId = CanIfCtrlCfg[ControllerId].CanIfCtrlCanCtrlRef ;
        ErrorStatus = Can_GetControllerErrorState( CanControllerId , ErrorStatePtr );
    }
    else
    {
        /*MISRA*/
    }
    return ErrorStatus ;
                                                                                                                                                                                                                                                                                                                                                                                                }



FUNC(Std_ReturnType,CANIF) CanIf_Transmit(VAR(PduIdType,AUTOMATIC) TxPduId,  CONSTP2VAR(PduInfoType,CANIF_CODE,AUTOMATIC) PduInfoPtr)
                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(Std_ReturnType, AUTOMATIC)          toBeReturned        = E_OK             ;
    VAR(Can_ControllerStateType,AUTOMATIC)  Can_ControllerState = 0                ;
    VAR(uint8_t, AUTOMATIC)                 HTH                 = 0                ;
    VAR(uint16_t, AUTOMATIC)                 HTHIndex           = 0                ;
    VAR(uint8_t, AUTOMATIC)                 CanifcfgIndex       = 0  ;
    VAR(uint8_t, AUTOMATIC)                 CanControllerId     = 0  ;
    VAR(Can_PduType,AUTOMATIC)              PduInfo             = {0}              ;


    if(CanIf_Global. initRun == (uint8_t)FALSE)
    {
        toBeReturned = E_NOT_OK;
    }
    else
    {
        if ( PduInfoPtr == NULL_PTR )
        {
            toBeReturned = E_NOT_OK;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
            Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_TRANSMIT_ID,CANIF_E_PARAM_POINTER);
#endif
        }
        else
        {
            if( IS_VALID_TX_PDU_ID(TxPduId) == FALSE )
            {
                toBeReturned = E_NOT_OK;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
                Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_TRANSMIT_ID,CANIF_E_INVALID_TXPDUID);
#endif
            }
            else
            {
                CanifcfgIndex = CanIfHthCfg[CanIfTxPduCfg[TxPduId].CanIfHthCfgRef].CanIfHthCanCtrlIdRef;

                CanControllerId = CanIfCtrlCfg[CanifcfgIndex].CanIfCtrlCanCtrlRef;
                if( ( Can_GetControllerMode( CanControllerId , &Can_ControllerState) == E_OK ) && (toBeReturned == E_OK ) )
                {
                    /* [SWS_CANIF_00317]
                     * The service CanIf_Transmit() shall not accept a transmit
                     * request, if the controller mode referenced by ControllerId is different to
                     * CAN_CS_STARTED and the channel mode at least for the transmit path is not online
                     * or offline active
                     */
                    if(Can_ControllerState == CAN_CS_STARTED)
                    {
                        if( IS_NOT_PDU_TRANSMIT_MODE() == TRUE )
                        {
                            toBeReturned = E_NOT_OK;
                        }
                        else
                        {
                            HTHIndex            = CanIfTxPduCfg[TxPduId].CanIfHthCfgRef  ;
                            PduInfo.sdu         = PduInfoPtr->SduDataPtr                 ;
                            PduInfo.length      = (uint8_t)PduInfoPtr->SduLength         ;
                            PduInfo.swPduHandle = TxPduId                                ;
                            PduInfo.id          = CanIfTxPduCfg[TxPduId].CanIfTxPduCanId ;

                            toBeReturned = Can_Write( HTHIndex , &PduInfo);
                        }
                    }
                    else
                    {
                        toBeReturned = E_NOT_OK ;
                    }
                    return toBeReturned;
                }
                else
                {

                }
            }
        }
    }
                                                                                                                                                                                                                                                                                                                                                                                                }
FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetPduMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR( CanIf_PduModeType, AUTOMATIC) PduModeRequest)
                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(uint8_t,AUTOMATIC)                  canIfDevError       = E_OK  ;
    VAR(Std_ReturnType,AUTOMATIC)           toBeReturned        = E_OK  ;
    VAR(Can_ControllerStateType,AUTOMATIC)  Can_ControllerState = 0     ;
    VAR(uint8_t,AUTOMATIC) CanControllerId                         = 0                             ;

    if(CanIf_Global.initRun == (uint8_t)TRUE)
    {
        if(IS_VALID_CONTROLLER_ID(ControllerId) == TRUE )
        {
            /*[SWS_CANIF_00874] The service CanIf_SetPduMode() shall not accept any request
             *                  and shall return E_NOT_OK, if the controller mode referenced by ControllerId
             *                  is not in state CAN_CS_STARTED
             */
            CanControllerId = CanIfCtrlCfg[ControllerId].CanIfCtrlCanCtrlRef ;
            toBeReturned = Can_GetControllerMode(CanControllerId, &Can_ControllerState);
            if(Can_ControllerState == CAN_CS_STARTED && toBeReturned == E_OK)
            {
                if(IS_PDU_MODE(PduModeRequest) == (uint8_t) TRUE)
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

    if(CanIf_Global.initRun == (uint8_t)TRUE)
    {
        if(IS_VALID_CONTROLLER_ID(ControllerId) == (uint8_t) TRUE )
        {
            if(PduModePtr != NULL_PTR)
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
FUNC(void ,CANIF_CODE) CanIf_RxIndication(CONSTP2VAR(Can_HwType,CANIF_CODE,AUTOMATIC) Mailbox, CONSTP2VAR(PduInfoType,CANIF_CODE,AUTOMATIC) PduInfoPtr)
                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(uint8_t,AUTOMATIC)     canIfDevError = E_OK ;
    VAR(uint8_t,AUTOMATIC)     PduIndex      = 0    ;
    VAR(uint8_t,AUTOMATIC)     CanifControllerIndex    = 0    ;
    VAR(uint8_t,AUTOMATIC)     PduMode       = 0    ;
    VAR(PduInfoType,AUTOMATIC) pduInfo              ;
    VAR(PduInfoType,AUTOMATIC) CanTpRxPdu           ;

    if(CanIf_Global.initRun == (uint8_t)TRUE )
    {
        /*SWS_CANIF_00421]   If CanIf was not initialized before calling CanIf_RxIndication(), CanIf shall not execute
                             Rx indication handling, when CanIf_RxIndication(), is called.*/
        if ( Mailbox == NULL_PTR || PduInfoPtr == NULL_PTR )
        {
            /*SWS_CANIF_00419] d If parameter PduInfoPtr or Mailbox of
          CanIf_RxIndication() has an invalid value, CanIf shall report development error code CANIF_E_PARAM_POINTER to the Det_ReportError 
          service of the DET module, when CanIf_RxIndication() is called*/

            canIfDevError =E_NOT_OK ;
#if CANIF_DEV_ERROR_DETECT == STD_ON
            Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_RX_INDICATION_ID,CANIF_E_PARAM_POINTER);
#endif 
        }
        else
        {
            if ( IS_VALID_HO(Mailbox->Ho) == FALSE  )
            {
                /*[SWS_CANIF_00416]  If parameter Mailbox->Hoh of CanIf_RxIndication() has an invalid value,
                                     CanIf shall report development error code CANIF_E_PARAM_HOH to the Det_ReportError service of
                                     the DET module, when CanIf_RxIndication() is called.*/
                canIfDevError =E_NOT_OK ;
#if CANIF_DEV_ERROR_DETECT == STD_ON
                Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_RX_INDICATION_ID,CANIF_E_PARAM_HOH);
#endif 
            }
            else
            {

                if (canIfDevError == E_OK )
                {
                    /*[SWS_CANIF_00423]  Configuration of CanIf_RxIndication(): Each Rx L-PDU (see ECUC_CanIf_00249) has to be configured with
    a corresponding receive indication service of an upper layer module (see [SWS_CANIF_00012]) which is called in
    CanIf_RxIndication().*/
                    /*[SWS_CANIF_00415]  Within the service CanIf_RxIndication() the CanIf
    routes this indication to the configured upper layer target service(s).*/
                    /* this loop to get the corresponding pdu index which has the same Can id */
                    for(PduIndex = 0 ; PduIndex < (uint8_t)MAX_NUM_RX_PDU ; PduIndex++)
                    {
                        CanifControllerIndex = CanIfHrhCfg[CanIfRxPduCfg[PduIndex].CanIfRxPduHrhIdRef].CanIfHrhCanCtrlIdRef ;
                        if ( (Mailbox ->CanId == CanIfRxPduCfg[PduIndex].CanIfRxPduCanId) &&  (CanIfCtrlCfg[CanifControllerIndex].CanIfCtrlCanCtrlRef == Mailbox->controllerlId) )
                        {
                            break ;
                        }
                        else
                        {
                            /*MISRA*/
                        }
                    }

                    if ( IS_VALID_RX_PDU(PduIndex) == TRUE )
                    {
                        PduMode    = CanIf_Global.channelData[CanifControllerIndex].PduMode;
                        if( ( ( PduMode == CANIF_OFFLINE) || (PduMode == CANIF_TX_OFFLINE) || (PduMode == CANIF_TX_OFFLINE_ACTIVE) ) )
                        {
                            /*Not Valid pdu mode */
                        }
                        else
                        {
                            if ( IS_NOT_VALID_DATA_LENGTH(PduInfoPtr->SduLength,CanIfRxPduCfg[PduIndex].CanIfRxPduCanIdType) == TRUE )
                            {
#if CANIF_PRIVATE_DATA_LENGTH_CHECK == TRUE
                                /*If CanIf_RxIndication() is called with invalid PduInfoPtr->SduLength, runtime error CANIF_E_INVALID_DATA_LENGTH is reported (see
                          [SWS_CANIF_00168]).*/
                                canIfDevError = E_NOT_OK ;
#if CANIF_DEV_ERROR_DETECT == STD_ON
                                Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_RX_INDICATION_ID,CANIF_E_INVALID_DATA_LENGTH);
#endif
#endif
                            }
                            else
                            {
                                /*Get the upper layer */
                                switch( CanIfRxPduCfg[PduIndex].CanIfRxPduUserRxIndicationUL)
                                {
                                case CAN_TP:
                                {
                                    CanTpRxPdu.SduLength = PduInfoPtr->SduLength;
                                    CanTpRxPdu.SduDataPtr = (uint8_t *)PduInfoPtr->SduDataPtr;
#if DUBGE == TRUE
                                    UARTprintf("CanifRx,Tp:%d\n",CanIfRxPduCfg[PduIndex].CanIfRxPduCanId);
#endif
#if 0
                                    /*TODO:Mapping between pduid  & CanIfRxPduId using ECUC */
                                    CanTp_RxIndication( CanIfRxPduCfg[PduIndex].CanIfRxPduId , &CanTpRxPdu );
#endif
                                    break;
                                }
                                case PDUR:
                                {
                                    pduInfo.SduLength  = PduInfoPtr->SduLength;
                                    pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
#if DUBGE == TRUE
                                    UARTprintf("CanifRx,pdur:%d\n",CanIfRxPduCfg[PduIndex].CanIfRxPduCanId);
#endif
#if 0
                                    PduR_CanIfRxIndication(CanIfRxPduCfg[PduIndex].CanIfRxPduId,&pduInfo);
#endif
                                    break;
                                }
                                case CDD:
                                    if ( CanIfRxPduCfg[PduIndex].CanIfRxPduUserRxIndicationName )
                                    {
                                        (CanIfRxPduCfg[PduIndex].CanIfRxPduUserRxIndicationName()) ;
                                    }

                                    else
                                    {
                                        /*MISRA*/
                                    }
                                default:
                                    break;
                                }/*Swich End */
                            }
                        }

                    }
                    else
                    {
                        /*[SWS_CANIF_00417]  If parameter Mailbox->CanId of CanIf_RxIndication() has an invalid value,
                CanIf shall report development error code CANIF_E_PARAM_CANID to the Det_ReportError service of the DET
                module, when CanIf_RxIndication() is called. */
                        canIfDevError =E_NOT_OK ;
#if CANIF_DEV_ERROR_DETECT == STD_ON
                        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_RX_INDICATION_ID,CANIF_E_PARAM_CANID);
#endif
                    }
                }
                else
                {
                    /*MISRA*/
                }
            }

        }
    }
                                                                                                                                                                                                                                                                                                                                                                                }
#endif


















FUNC(void,CANIF_CODE) CanIf_TxConfirmation(VAR(PduIdType,AUTOMATIC) CanTxPduId)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(uint8_t,AUTOMATIC) upperLayer = CanIfTxPduCfg[CanTxPduId].CanIfTxPduUserTxConfirmationUL;
    if( CanIf_Global.initRun == (uint8_t)TRUE )
    {
        if( IS_VALID_TX_PDU_ID(CanTxPduId) == (uint8_t)TRUE )
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
                /*TODO:Mapping between pduid  & CanIfTxPduId using ECUC */
                CanTp_TxConfirmation(CanIfTxPduCfg[CanTxPduId].CanIfTxPduId, E_OK);
#endif
                break;
            case PDUR:
#if 0
                PDUr_TxConfirmation(CanIfTxPduCfg[CanTxPduId].CanIfTxPduId, E_OK);
#endif
                break;
            default:
                break;
            }
        }
        else
        {
#if CANIF_DEV_ERROR_DETECT == STD_ON
            Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_TX_CONFIRMATION_ID,CANIF_E_PARAM_LPDU);
#endif
        }
    }
    else
    {
    }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }



