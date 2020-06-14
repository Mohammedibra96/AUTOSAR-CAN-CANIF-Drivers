#include "include/CanIf.h"
#include "Canif_cfg.h"


VAR(CanIfRxPduCfg_s ,AUTOMATIC) CanIfRxPduCfg[MAX_NUM_RX_PDU] = {
	{
		.CanIfRxPduCanId = 1,
		.CanIfRxPduDataLength = 4,
		.CanIfRxPduUserRxIndicationUL = PDUR,
		.CanIfRxPduHrhIdRef = 0,
		.CanIfRxPduRef = 3
	},
	{
		.CanIfRxPduCanId = 2,
		.CanIfRxPduDataLength = 8,
		.CanIfRxPduUserRxIndicationUL = CAN_TP,
		.CanIfRxPduHrhIdRef = 1,
		.CanIfRxPduRef = 4
	},
	{
		.CanIfRxPduCanId = 3,
		.CanIfRxPduDataLength = 8,
		.CanIfRxPduUserRxIndicationUL = PDUR,
		.CanIfRxPduHrhIdRef = 2,
		.CanIfRxPduRef = 5
	}
};
VAR(CanIfTxPduCfg_s ,AUTOMATIC) CanIfTxPduCfg[MAX_NUM_TX_PDU] = {
	{
		.CanIfTxPduCanId = 6,
		.CanIfTxPduCanIdType = STANDARD_CAN,
		.CanIfTxPduId = 5,
		.CanIfRxPduUserRxConfirmationUL = ,
		.CanIfHthCfgRef = 0
	},
	{
		.CanIfTxPduCanId = 7,
		.CanIfTxPduCanIdType = STANDARD_CAN,
		.CanIfTxPduId = 9,
		.CanIfRxPduUserRxConfirmationUL = ,
		.CanIfHthCfgRef = 1
	},
};



VAR(CanIfHrhCfg_s ,AUTOMATIC) CanIfHrhCfg[MAX_NUM_HRH]={
	{
		.CanIfHrhSoftwareFilter = TRUE,
		.CanIfHrhCanCtrlIdRef = 0,
		.CanIfHrhIdSymRef = 2
	},
	{
		.CanIfHrhSoftwareFilter = FALSE,
		.CanIfHrhCanCtrlIdRef = 0,
		.CanIfHrhIdSymRef = 3
	},
	{
		.CanIfHrhSoftwareFilter = FALSE,
		.CanIfHrhCanCtrlIdRef = 0,
		.CanIfHrhIdSymRef = 4
	}
};
VAR(CanIfHthCfg_s ,AUTOMATIC) CanIfHthCfg[MAX_NUM_HTH] = {
	{
		.CanIfHthCanCtrlIdRef = 0,
		.CanIfHthIdSymRef = 1
	},
	{
		.CanIfHthCanCtrlIdRef = 0,
		.CanIfHthIdSymRef = 0
	}
};
