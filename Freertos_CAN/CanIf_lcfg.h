#ifndef CANIF_LCFG_H_
#define CANIF_LCFG_H_

extern VAR(CanIfRxPduCfg_s ,AUTOMATIC) CanIfRxPduCfg[MAX_NUM_RX_PDU];
extern VAR(CanIfTxPduCfg_s ,AUTOMATIC) CanIfTxPduCfg[MAX_NUM_TX_PDU];
extern VAR(CanIfHthCfg_s ,AUTOMATIC) CanIfHthCfg[MAX_NUM_HTH];
extern VAR(CanIfHrhCfg_s ,AUTOMATIC) CanIfHrhCfg[MAX_NUM_HRH];
extern VAR(CanIfCtrlCfg_s,AUTOMATIC) CanIfCtrlCfg[MAX_CANIF_CTRL];

#endif /* CANIF_LCFG_H_ */
