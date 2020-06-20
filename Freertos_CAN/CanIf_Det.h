#ifndef CANIF_DET_H_
#define CANIF_DET_H_

/* Det defines */

#define CANIF_E_PARAM_CANID                 (uint8_t) 10
#define CANIF_E_PARAM_HOH                   (uint8_t) 12
#define CANIF_E_PARAM_LPDU                  (uint8_t) 13
#define CANIF_E_PARAM_CONTROLLERID          (uint8_t) 15
#define CANIF_E_PARAM_WAKEUPSOURCE          (uint8_t) 16
#define CANIF_E_PARAM_TRCV                  (uint8_t) 17
#define CANIF_E_PARAM_TRCVMODE              (uint8_t) 18
#define CANIF_E_PARAM_TRCVWAKEUPMODE        (uint8_t) 19
#define CANIF_E_PARAM_CTRLMODE              (uint8_t) 21
#define CANIF_E_PARAM_PDU_MODE              (uint8_t) 22
#define CANIF_E_PARAM_POINTER               (uint8_t) 20
#define CANIF_E_UNINIT                      (uint8_t) 30
#define CANIF_E_INVALID_TXPDUID             (uint8_t) 50
#define CANIF_E_INVALID_RXPDUID             (uint8_t) 60
#define CANIF_E_INIT_FAILED                 (uint8_t) 80
#define CANIF_E_INVALID_DATA_LENGTH         (uint8_t) 61


#define CANIF_INIT_ID                        (uint8_t)0x01
#define CANIF_INIT_CONTROLLER_ID             (uint8_t)0x02
#define CANIF_SET_CONTROLLER_MODE_ID         (uint8_t)0x03
#define CANIF_GET_CONTROLLER_MODE_ID         (uint8_t)0x04
#define CANIF_GET_CONTROLLER_ERROR_STATE_ID  (uint8_t)0x4B
#define CANIF_TRANSMIT_ID                    (uint8_t)0x05
#define CANIF_READTXPDUDATA_ID               (uint8_t)0x06
#define CANIF_READTXNOTIFSTATUS_ID           (uint8_t)0x07
#define CANIF_READRXNOTIFSTATUS_ID           (uint8_t)0x08
#define CANIF_SET_PDU_MODE_ID                (uint8_t)0x09
#define CANIF_GET_PDU_MODE_ID                (uint8_t)0x0A
#define CANIF_CHECKVALIDATION_ID             (uint8_t)0x12
#define CANIF_TX_CONFIRMATION_ID             (uint8_t)0x13
#define CANIF_RX_INDICATION_ID               (uint8_t)0x14

#define CANIF_MODULE_ID                      (uint16_t)MODULE_ID_CANIF
#define CANIF_INSTANCE_ID                    (uint8_t)0



#endif /* CANIF_DET_H_ */
