
#ifndef CANIF_H_
#define CANIF_H_


#include "std_types.h"
#include "Modules.h"
#if defined(USE_PDUR)
#include "PduR.h"
#endif

#if CANIF_DEV_ERROR_DETECT == STD_ON
#include "Com.h"
#endif

#define CANIF_VENDOR_ID          (VENDOR_ID_ARCCORE)
#define CANIF_MODULE_ID          (MODULE_ID_CANIF)



#if defined(USE_DET)
#include "Det.h"
#endif
#include "CanIf_Types.h"
#include "CanIf_Cfg.h"

#define IS_PDU_MODE(PDU_MODE)   (PDU_MODE == CANIF_OFFLINE) || (PDU_MODE == CANIF_TX_OFFLINE) || (PDU_MODE == CANIF_TX_OFFLINE_ACTIVE) || (PDU_MODE == CANIF_ONLINE)




FUNC(void,CANIF_CODE) CanIf_Init(CONSTP2VAR(CanIf_ConfigType_s,CANIF_CODE,AUTOMATIC) ConfigPtr);
FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode);

FUNC(Std_ReturnType,CANIF_CODE) CanIf_GetControllerMode(VAR(uint8_t ,AUTOMATIC) ControllerId, Can_ControllerStateType* ControllerModePtr);


FUNC(Std_ReturnType,CANIF) CanIf_GetControllerErrorState(VAR(uint8_t,AUTOMATIC) ControllerId,P2VAR( Can_ErrorStateType,CANIF_CODE,AUTOMATIC) ErrorStatePtr);

FUNC(Std_ReturnType,CANIF) CanIf_GetControllerErrorState(VAR(uint8_t,AUTOMATIC) ControllerId,P2VAR( Can_ErrorStateType,CANIF_CODE,AUTOMATIC) ErrorStatePtr);

FUNC(Std_ReturnType,CANIF) CanIf_Transmit(VAR(PduIdType,AUTOMATIC) TxPduId,  CONSTP2VAR(PduInfoType,CANIF_CODE,AUTOMATIC) PduInfoPtr);

FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetPduMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR( CanIf_PduModeType, AUTOMATIC) PduModeRequest);

FUNC(Std_ReturnType ,CANIF) CanIf_GetPduMode(uint8_t ControllerId, CanIf_PduModeType* PduModePtr);

FUNC(void,CANIF_CODE) CanIf_RxIndication(const Can_HwType* Mailbox, const PduInfoType *PduInfoPtr);

FUNC(void,CANIF_CODE) CanIf_TxConfirmation(PduIdType CanTxPduId);



#endif
