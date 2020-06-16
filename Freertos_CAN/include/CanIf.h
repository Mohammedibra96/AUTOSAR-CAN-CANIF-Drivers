
#ifndef CANIF_H_
#define CANIF_H_


#include "std_types.h"
#include "Modules.h"
#if defined(USE_PDUR)
#include "PduR.h"
#endif

#if defined(USE_COM)
#include "Com.h"
#endif

#define CANIF_TESTING_TRANSMIT_NADAANDAHMED 1
#define CANIF_VENDOR_ID          (VENDOR_ID_ARCCORE)
#define CANIF_MODULE_ID          (MODULE_ID_CANIF)
#define CANIF_AR_MAJOR_VERSION   3
#define CANIF_AR_MINOR_VERSION   1
#define CANIF_AR_PATCH_VERSION   5

#define CANIF_SW_MAJOR_VERSION   1
#define CANIF_SW_MINOR_VERSION   3
#define CANIF_SW_PATCH_VERSION   0

/* Configuration Macros */

/*From the configuration tool as the sum of all the buffers*/
#define CANIF_BUFFER_POOL_SIZE 40

#if defined(USE_DET)
#include "Det.h"
#endif
#include "CanIf_Types.h"
#include "CanIf_Cfg.h"

#define IS_PDU_MODE(PDU_MODE)   (PDU_MODE == CANIF_OFFLINE) || (PDU_MODE == CANIF_TX_OFFLINE) || (PDU_MODE == CANIF_TX_OFFLINE_ACTIVE) || (PDU_MODE == CANIF_ONLINE)




FUNC(void,CANIF_CODE) CanIf_Init(const CanIf_ConfigType* ConfigPtr);
FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode);

FUNC(Std_ReturnType,CANIF_CODE) CanIf_GetControllerMode(VAR(uint8_t ,AUTOMATIC) ControllerId, Can_ControllerStateType* ControllerModePtr);


FUNC(Std_ReturnType,CANIF) CanIf_GetControllerErrorState(uint8_t ControllerId, Can_ErrorStateType* ErrorStatePtr);

FUNC(Std_ReturnType,CANIF) CanIf_GetControllerErrorState(uint8_t ControllerId, Can_ErrorStateType* ErrorStatePtr);

FUNC(Std_ReturnType,CANIF) CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);

FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetPduMode(uint8_t ControllerId, CanIf_PduModeType PduModeRequest);

FUNC(Std_ReturnType ,CANIF) CanIf_GetPduMode(uint8_t ControllerId, CanIf_PduModeType* PduModePtr);

FUNC(void,CANIF_CODE) CanIf_RxIndication(const Can_HwType* Mailbox, const PduInfoType *PduInfoPtr);

FUNC(void,CANIF_CODE) CanIf_TxConfirmation(PduIdType CanTxPduId);



#endif
