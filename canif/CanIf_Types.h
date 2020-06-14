
#ifndef CANIF_TYPES_H_
#define CANIF_TYPES_H_

#include "include/Can_GeneralTypes.h"
#include "include/Can_Cfg.h"
#include "CanIf_Cfg.h"
#include "stdint.h"
#include "Platform_Types.h"
#include "ComStack_Types.h"
#include "stdbool.h"


/********************************************************************************************************/
/********************************************************************************************************/
/*************************************************      MACROS  *****************************************/
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/


#define EXTENDED_CAN                              0
#define EXTENDED_FD_CAN                           1
#define STANDARD_CAN                              2
#define STANDARD_FD_CAN                           3


/** Channel is in the offline mode ==> no transmission or reception */
#define CANIF_GET_OFFLINE                         0
/** Receive path of the corresponding channel is enabled and
*transmit path is disabled */
#deifne CANIF_GET_RX_ONLINE                       1
/** Transmit path of the corresponding channel is enabled and
*receive path is disabled */
#deifne CANIF_GET_TX_ONLINE                       2
/** Channel is in the online mode ==> full operation mode */
#deifne CANIF_GET_ONLINE                          3
/** Transmit path of the corresponding channel is in
*the offline mode ==> transmit notifications are processed but
*transmit requests are blocked. The receiver path is disabled. */
#deifne CANIF_GET_OFFLINE_ACTIVE                  4
/** Transmit path of the corresponding channel is in the offline
*active mode ==> transmit notifications are processed but transmit
*requests are blocked. The receive path is enabled. */
#define CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE        5



/*
The PduMode of a channel defines its transmit or receive activity.
Communication direction (transmission and/or reception) of the channel can
be controlled separately or together by upper layers.
*/
#define CANIF_OFFLINE                             1

//Transmit path of the corresponding channel is disabled. The receive path is enabled.
#define CANIF_TX_OFFLINE                          2

//Transmit path of the corresponding channel is in offline active mode
//The receive path is disabled. This mode requires CanIfTxOfflineActiveSupport = TRUE.
#define CANIF_TX_OFFLINE_ACTIVE                    3

//Transmit and receive path of the corresponding channel are enabled => full operation mode
#define CANIF_ONLINE                               4



/********************************************************************************************************/
/********************************************************************************************************/
/************************************     Statnadrd types in SWS     ************************************/
/********************************************************************************************************/
/********************************************************************************************************/


typedef enum
{
    CANIF_OFFLINE,                  /*Transmit and receive path of the corresponding channel are disabled => no communicationmode*/
    CANIF_TX_OFFLINE,               /* Transmit path of the corresponding channel is disabled. The receive path is enabled */
    CANIF_TX_OFFLINE_ACTIVE,        /* Transmit path of the corresponding channel is in offline active mode. 
                                       The receive path is disabled. This mode requires CanIfTxOfflineActiveSupport = TRUE */
    CANIF_ONLINE                    /* Transmit and receive path of the corresponding channel are enabled => full operation mode */
}CanIf_PduModeType;


typedef struct{
    /* This parameter abstracts from the CAN Driver specific parameter
     * Controller. Each controller of all connected CAN Driver modules shall
     * be assigned to one specific ControllerId of the CanIf. Range:
     * 0..number of configured controllers of all CAN Driver modules */
    uint8_t CanIfCtrlId               ;

    CanController* CanIfCtrlCanCtrlRef;
}CanIfCtrlCfg_s;


typedef struct {

    /** Selects the hardware receive objects by using the HRH range/list from
     *  CAN Driver configuration to define, for which HRH a software filtering has
     *  to be performed at during receive processing. True: Software filtering is
     *  enabled False: Software filtering is disabled */
    bool CanIfHrhSoftwareFilter;

    /** Reference to controller Id to which the HRH belongs to. A controller can
     *  contain one or more HRHs. */
    uint8_t CanIfHrhCanCtrlIdRef;

    /* The parameter refers to a particular HRH object in the CanDrv
     * configuration */
    uint8_t CanIfHrhIdSymRef;
    
} CanIfHrhCfg_s;

/* CanIfHrhCfg_s CanIfHrhCfg[MAX_HRH_OBJECTS];  */


typedef struct{
    /* Reference to controller Id to which the HTH belongs to. A controller can contain one or more HTHs*/
    uint8_t CanIfHthCanCtrlIdRef;

    /* The parameter refers to a particular HTH object in the CanDrvconfiguration */
    uint8_t CanIfHthIdSymRef;

}CanIfHthCfg_s;

/*CanIfHthCfg_s  CanIfHthCfg[MAX_HTH_OBJECTS];   */



typedef struct {
  /** CAN Identifier of Receive CAN L-PDUs used by the CAN Interface.
    * Exa: Software Filtering. This parameter is used if exactly one Can
    * Identifier is assigned to the Pdu. If a range is assigned then the
    * CanIfRxPduCanIdRange parameter shall be used.
    * Range: 11 Bit For Standard CAN Identifier ... 29 Bit For Extended CAN identifier */
    uint32_t CanIfRxPduCanId;  //



  /** CAN Identifier of receive CAN L-PDUs used by the CAN Driver for CAN L-
   *  PDU transmission.
     *  EXTENDED_CAN  The CANID is of type Extended (29 bits)
   *  STANDARD_CAN  The CANID is of type Standard (11 bits) */
    CanIfTxPduCanIdType CanIfRxPduCanIdType; //


  /** Data Length code of received CAN L-PDUs used by the CAN Interface.
   *  Exa: DLC check. The data area size of a CAN L-PDU can have a range
   *  from 0 to 8 bytes.*/
    uint16_t CanIfRxPduDataLength;  //



  /** Enables and disables the Rx buffering for reading of received L-PDU data.
   *  True: Enabled False: Disabled */
    bool CanIfRxPduReadData;  //


    /*This parameter defines the upper layer (UL) module to which the
        indication of the successfully received CANRXPDUID has to be routed
        via <User_RxIndication>. This <User_RxIndication> has to be invoked
        when the indication of the configured CANRXPDUID will be received
        by an Rx indication event from the CAN Driver module. If no upper
        layer (UL) module is configured, no <User_RxIndication> has to be
        called in case of an Rx indication event of the CANRXPDUID from the
        CAN Driver module. */
    CanIfRxPduUserRxConfirmationUL_t CanIfRxPduUserRxIndicationUL;





  /** The HRH to which Rx L-PDU belongs to, is referred through this
   *  parameter. */
    uint8_t  CanIfRxPduHrhIdRef;  //Refrence as index 

  /** Reference to the "global" Pdu structure to allow harmonization of handle
   *  IDs in the COM-Stack. */
   //Change the refrence to struct with index refrence


   
    uint32_t  CanIfRxPduRef;

}CanIfRxPduCfg_s;


typedef struct {

    /** CAN Identifier of transmit CAN L-PDUs used by the CAN Driver for CAN L-
     *  PDU transmission. Range: 11 Bit For Standard CAN Identifier ... 29 Bit For
     *  Extended CAN identifier */
    uint32_t CanIfTxPduCanId;   ///

/*
*Type of CAN Identifier of the transmit CAN L-PDU used by the CAN
*Driver module for CAN L-PDU transmission.
* Standard or Extended or FD
*/
    CanIfTxPduCanIdType CanIfTxPduCanIdType; ///

    /** ECU wide unique, symbolic handle for transmit CAN L-PDU. The
     *  CanIfCanTxPduId is configurable at pre-compile and post-built time.
     *  Range: 0..max. number of CantTxPduIds   PduIdType   CanTxPduId; */
    uint32_t CanIfTxPduId; 


    CanIfRxPduUserRxConfirmationUL CanIfTxPduUserTxConfirmationUL_i;

    /** Reference to the "global" Pdu structure to allow harmonization of handle
     *  IDs in the COM-Stack. */


//Change the refrence to struct with index refrence

    uint32_t CanIfTxPduRef; 

//Adding Refrence to CanIfHthCfg_s  

uint8_t CanIfHthCfgRef;

} CanIfTxPduCfg_s;


/****************************   Types Not in SWS    *************************************/

typedef struct{
    Can_ControllerStateType Controller_Mode;
    CanIf_ChannelGetModeType PduMode;
}CanIf_ChannelPrivateType;

typedef struct {
    boolean initRun;
    CanIf_ChannelPrivateType channelData[CANIF_CHANNEL_CNT];
}CanIf_GlobalType;



#endif /* CANIF_TYPES_H_ */
