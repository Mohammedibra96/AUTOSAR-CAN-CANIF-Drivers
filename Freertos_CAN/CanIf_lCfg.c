#include "include/CanIf.h"
#include "include/Canif_cfg.h"


VAR(CanIfRxPduCfg_s ,AUTOMATIC) CanIfRxPduCfg[MAX_NUM_RX_PDU] = {
                                                                 {
                                                                  .CanIfRxPduCanId      = 1           ,
                                                                  .CanIfRxPduDataLength = 4           ,
                                                                  .CanIfRxPduUserRxIndicationUL = PDUR,
                                                                  .CanIfRxPduHrhIdRef = 0             ,
                                                                  .CanIfRxPduId = 2
                                                                 },
                                                                 {
                                                                  .CanIfRxPduCanId = 2 ,
                                                                  .CanIfRxPduDataLength = 8,
                                                                  .CanIfRxPduUserRxIndicationUL = CAN_TP,
                                                                  .CanIfRxPduHrhIdRef = 1,
                                                                  .CanIfRxPduId = 3
                                                                 },
                                                                 {
                                                                  .CanIfRxPduCanId = 3,
                                                                  .CanIfRxPduDataLength = 8,
                                                                  .CanIfRxPduUserRxIndicationUL = PDUR,
                                                                  .CanIfRxPduHrhIdRef = 2,
                                                                  .CanIfRxPduId = 5
                                                                 },
                                                                 {
                                                                     .CanIfRxPduCanId = 4,
                                                                             .CanIfRxPduDataLength = 8,
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
                                                         .CanIfHrhSoftwareFilter = TRUE,
                                                         .CanIfHrhCanCtrlIdRef = CANIF_CHANNEL_1,
                                                         .CanIfHrhIdSymRef = 4
                                                        },
                                                        {
                                                         .CanIfHrhSoftwareFilter = FALSE,
                                                         .CanIfHrhCanCtrlIdRef = CANIF_CHANNEL_1,
                                                         .CanIfHrhIdSymRef = 5
                                                        },
                                                        {
                                                         .CanIfHrhSoftwareFilter = FALSE,
                                                         .CanIfHrhCanCtrlIdRef = CANIF_CHANNEL_1,
                                                         .CanIfHrhIdSymRef = 6
                                                        },
                                                        {
                                                            .CanIfHrhSoftwareFilter = FALSE,
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
