
#ifndef CANIF_H_
#define CANIF_H_


#include <Modules.h>
#include "std_types.h"
#if defined(USE_PDUR)
#include <PduR.h>
#endif
#if defined(USE_COM)
#include <Com.h>
#endif

#define CANIF_VENDOR_ID          (VENDOR_ID_ARCCORE)
#define CANIF_MODULE_ID          (MODULE_ID_CANIF)



#if defined(USE_DET)
#include <Det.h>
#endif
#include <CanIf_Types.h>
#include <CanIf_Cfg.h>

#define IS_PDU_MODE(PDU_MODE)   (PDU_MODE == CANIF_OFFLINE) || (PDU_MODE == CANIF_TX_OFFLINE) || (PDU_MODE == CANIF_TX_OFFLINE_ACTIVE) || (PDU_MODE == CANIF_ONLINE)




FUNC(void,CANIF_CODE) CanIf_Init(CONSTP2VAR(CanIf_ConfigType_s,CANIF_CODE,AUTOMATIC) ConfigPtr);
FUNC(void,CANIF_CODE) CanIf_DeInit(void);
FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode);
FUNC(Std_ReturnType,CANIF_CODE) CanIf_GetControllerMode(VAR(uint8_t ,AUTOMATIC) ControllerId, Can_ControllerStateType* ControllerModePtr);
FUNC(Std_ReturnType,CANIF) CanIf_GetControllerErrorState(VAR(uint8_t,AUTOMATIC) ControllerId,P2VAR( Can_ErrorStateType,CANIF_CODE,AUTOMATIC) ErrorStatePtr);
FUNC(Std_ReturnType,CANIF) CanIf_Transmit(VAR(PduIdType,AUTOMATIC) TxPduId,  CONSTP2VAR(PduInfoType,CANIF_CODE,AUTOMATIC) PduInfoPtr);
FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetPduMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR( CanIf_PduModeType, AUTOMATIC) PduModeRequest);
FUNC(Std_ReturnType ,CANIF) CanIf_GetPduMode(VAR(uint8_t,AUTOMATIC) ControllerId,P2VAR(CanIf_PduModeType,CANIF_CODE,AUTOMATIC)  PduModePtr);
FUNC(void ,CANIF_CODE) CanIf_RxIndication(CONSTP2VAR(Can_HwType,CANIF_CODE,AUTOMATIC) Mailbox, CONSTP2VAR(PduInfoType,CANIF_CODE,AUTOMATIC) PduInfoPtr);

FUNC(void,CANIF_CODE) CanIf_TxConfirmation(VAR(PduIdType,AUTOMATIC) CanTxPduId);



#endif
