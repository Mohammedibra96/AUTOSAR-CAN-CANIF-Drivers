
#ifndef CANIF_CFG_H_
#define CANIF_CFG_H_


#define MAX_NUM_RX_PDU                      5
#define MAX_NUM_TX_PDU                      4

#define MAX_NUM_HTH                         5
#define MAX_NUM_HRH                         4


#define CANIF_DEV_ERROR_DETECT                   STD_ON

/* These macros shall be set to TRUE for enabling CanIf_RxIndication */
#define CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API TRUE
#define CANIF_RXPDU_READ_NOTIFYSTATUS            TRUE


#endif /* CANIF_CFG_H_ */
