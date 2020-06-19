#include <CanIf.h>
#include "Canif_cfg.h"

/* TODO:Check from ENg.ABdel7ai CanIfRxPduId can be generated from the tool or not  */

VAR(CanIfRxPduCfg_s ,AUTOMATIC) CanIfRxPduCfg[MAX_NUM_RX_PDU] = {
                                                                 {
                                                                  .CanIfRxPduCanIdType = STANDARD_CAN,
                                                                  .CanIfRxPduCanId      = 1           ,
                                                                  .CanIfRxPduDataLength = 4           ,
                                                                  .CanIfRxPduUserRxIndicationName=NULL_PTR,
                                                                  .CanIfRxPduUserRxIndicationUL = PDUR,
                                                                  .CanIfRxPduHrhIdRef = 0             ,
                                                                  .CanIfRxPduId = 2
                                                                 },
                                                                 {
                                                                  .CanIfRxPduCanIdType = STANDARD_CAN,
                                                                  .CanIfRxPduCanId = 2 ,
                                                                  .CanIfRxPduDataLength = 8,
                                                                  .CanIfRxPduUserRxIndicationName=NULL_PTR,
                                                                  .CanIfRxPduUserRxIndicationUL = CAN_TP,
                                                                  .CanIfRxPduHrhIdRef = 1,
                                                                  .CanIfRxPduId = 3
                                                                 },
                                                                 {
                                                                  .CanIfRxPduCanIdType = STANDARD_CAN,
                                                                  .CanIfRxPduCanId = 3,
                                                                  .CanIfRxPduDataLength = 8,
                                                                  .CanIfRxPduUserRxIndicationName=NULL_PTR,
                                                                  .CanIfRxPduUserRxIndicationUL = PDUR,
                                                                  .CanIfRxPduHrhIdRef = 2,
                                                                  .CanIfRxPduId = 5
                                                                 },
                                                                 {
                                                                  .CanIfRxPduCanIdType = STANDARD_CAN,
                                                                  .CanIfRxPduCanId = 4,
                                                                  .CanIfRxPduDataLength = 8,
                                                                  .CanIfRxPduUserRxIndicationName=NULL_PTR,
                                                                  .CanIfRxPduUserRxIndicationUL = PDUR,
                                                                  .CanIfRxPduHrhIdRef = 3,
                                                                  .CanIfRxPduId = 5
                                                                 }
};

VAR(CanIfTxPduCfg_s ,AUTOMATIC) CanIfTxPduCfg[MAX_NUM_TX_PDU] = {
                                                                 {
                                                                  .CanIfTxPduCanId = 1,
                                                                  .CanIfTxPduCanIdType = STANDARD_CAN,
                                                                  .CanIfTxPduId = 5,
                                                                  .CanIfTxPduUserTxConfirmationUL = CAN_TP,
                                                                  .CanIfHthCfgRef = 0
                                                                 },
                                                                 {
                                                                  .CanIfTxPduCanId = 2,
                                                                  .CanIfTxPduCanIdType = STANDARD_CAN,
                                                                  .CanIfTxPduId = 6,
                                                                  .CanIfTxPduUserTxConfirmationUL = PDUR,
                                                                  .CanIfHthCfgRef = 1
                                                                 },
                                                                 {
                                                                  .CanIfTxPduCanId = 3,
                                                                  .CanIfTxPduCanIdType = STANDARD_CAN,
                                                                  .CanIfTxPduId = 7,
                                                                  .CanIfTxPduUserTxConfirmationUL = PDUR,
                                                                  .CanIfHthCfgRef = 2
                                                                 },
                                                                 {
                                                                  .CanIfTxPduCanId = 4,
                                                                  .CanIfTxPduCanIdType = STANDARD_CAN,
                                                                  .CanIfTxPduId = 8,
                                                                  .CanIfTxPduUserTxConfirmationUL = CAN_TP,
                                                                  .CanIfHthCfgRef = 3
                                                                 }

};



VAR(CanIfHrhCfg_s ,AUTOMATIC) CanIfHrhCfg[MAX_NUM_HRH]={
                                                        {

                                                         .CanIfHrhCanCtrlIdRef = CANIF_CHANNEL_1,
                                                         .CanIfHrhIdSymRef = 4
                                                        },
                                                        {
                                                         .CanIfHrhCanCtrlIdRef = CANIF_CHANNEL_1,
                                                         .CanIfHrhIdSymRef = 5
                                                        },
                                                        {
                                                         .CanIfHrhCanCtrlIdRef = CANIF_CHANNEL_1,
                                                         .CanIfHrhIdSymRef = 6
                                                        },
                                                        {
                                                         .CanIfHrhCanCtrlIdRef = CANIF_CHANNEL_1,
                                                         .CanIfHrhIdSymRef = 7
                                                        }
};


VAR(CanIfHthCfg_s ,AUTOMATIC) CanIfHthCfg[MAX_NUM_HTH] = {
                                                          {
                                                           .CanIfHthCanCtrlIdRef = CANIF_CHANNEL_1,
                                                           .CanIfHthIdSymRef = 0
                                                          },
                                                          {
                                                           .CanIfHthCanCtrlIdRef = CANIF_CHANNEL_1,
                                                           .CanIfHthIdSymRef = 1
                                                          },
                                                          {
                                                           .CanIfHthCanCtrlIdRef = CANIF_CHANNEL_1,
                                                           .CanIfHthIdSymRef = 2
                                                          },
                                                          {
                                                           .CanIfHthCanCtrlIdRef = CANIF_CHANNEL_1,
                                                           .CanIfHthIdSymRef = 3
                                                          }
};
