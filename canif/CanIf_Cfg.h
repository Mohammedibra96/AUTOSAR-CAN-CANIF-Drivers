/*
 * CanIf_Cfg.h
 *
 *  Created on: Jun 23, 2019
 *      Author: leenovoz510
 */

#ifndef CANIF_CFG_H_
#define CANIF_CFG_H_


#define CANIF_CONTROLLER_ID_Controller_1    (CANIF_Channel_1)
#define MAX_NUM_RX_PDU                      5
#define MAX_NUM_TX_PDU                      4

#define MAX_NUM_HTH                         5
#define MAX_NUM_HRH                         4


#define CANIF_DEV_ERROR_DETECT   TRUE




typedef enum 
{
	CANIF_Channel_1,
    CANIF_Channel_2,
	CANIF_CHANNEL_CNT
} CanIf_ChannelIdType;



typedef enum {
    CAN_TP ,
    CDD,
    CAN_NM,
    J1939TP,
    PDUR,
    XCP,
    J1939NM,
    CAN_TSYN
}CanIfRxPduUserRxConfirmationUL;

typedef enum{
    USE_PDUR,
    USE_TP
}User_RxIndication;

typedef enum 
{
    BINARY,
    INDEX,
    LINEAR,
    TABLE,
    MASK
    
}SoftwareFilterType ;



typedef enum {
    HWObj_1,
    HWObj_2,
    HWObj_3,
    HWObj_4,
    HWObj_5,
    NUM_OF_HRHS
} Can_HRHType;


#endif /* CANIF_CFG_H_ */
