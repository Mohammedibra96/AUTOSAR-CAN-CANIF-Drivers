
/***********************************************************
 *
 * File Name: Com_Types.h
 *
 * Author: AUTOSAR COM Team 
 * 
 * Date Created: 6 March 2019	
 * 
 * Version	: 01	
 * 
 ************************************************************/

#ifndef _COM_TYPES_H_
#define _COM_TYPES_H_

#include "Std_Types.h"

/* Signal object identifier */
typedef uint16 Com_SignalIdType;
typedef uint16 Com_SignalGroupIdType;
typedef uint16 Com_SignalGeneral;
typedef uint16 EcucFloatParamDef;

/* Available Transmission modes for I-PDU */
typedef enum {
    DIRECT,
    MIXED,
    NONE,
    PERIODIC
} ComTxModeMode_type;

/* The direction defines if this I-PDU, and therefore the contributing signals and signal groups, shall be sent or received */
typedef enum {
    RECEIVE,
    SEND
} ComIPduDirection_type;

/* Defines if a write access to this signal can trigger the transmission of the correspon-ding I-PDU.
 * If the I-PDU is triggered, depends also on the transmission mode of the corresponding I-PDU. */
typedef enum {
    PENDING,
    TRIGGERED,
    TRIGGERED_ON_CHANGE,
    TRIGGERED_ON_CHANGE_WITHOUT_REPETITION,
    TRIGGERED_WITHOUT_REPETITION
}ComTransferProperty_type;

typedef enum {
    DEFERRED,
    IMMEDIATE
}ComIPduSignalProcessing_type;

typedef enum {
    NORMAL,
    TP
}ComIPduType_type;

typedef enum {
    CONFIRMATION,
    TRANSMIT,
    TRIGGERTRANSMIT
}ComTxIPduClearUpdateBit_type;

/*ComDataInvalidAction */
typedef enum
{
    NOTIFY,
    REPLACE
}ComDataInvalidAction_type;


/**/
typedef enum{
    Signal,
    Group
}SignalType;
/* Configuration structure for Tx-mode for I-PDUs. */
typedef struct
{
    /* Transmission mode for this IPdu. */
    const ComTxModeMode_type ComTxModeMode;
    /*
     * Defines the Number of repetition
     * in case ComTxModeMode is configured to DIRECT or MIXED
     */
    const uint8 ComTxModeNumberOfRepetitions;
    /*
     * Defines the repetition period in ms
     * in case ComTxModeNumberOfRepetitions is configured greater than or equal to 1
     * and ComTxModeMode is configured to DIRECT or MIXED
     */
    const uint16 ComTxModeRepetitionPeriod;
    /*
     * Defines the repetition period in ms
     * in case ComTxModeMode is configured to PERIODIC or MIXED
     */
    const uint16 ComTxModeTimePeriod;
} ComTxMode_type;

/* ComFilter evaluates to true */
typedef struct 
{
    const ComTxMode_type ComTxMode;
} ComTxModeTrue_type;

/* ComFilter evaluates to ComTxModeFalse */
typedef struct 
{
    const ComTxMode_type ComTxMode;
} ComTxModeFalse_type;

/* This container must be included if COM_IPDU_DIRECTION is configured to SEND */
typedef struct 
{
    /* Minimum delay between successive transmissions of the IPdu */
    const uint32 ComMinimumDelayTime;

    ComTxIPduClearUpdateBit_type ComTxIPduClearUpdateBit;

    /* COM will fill unused areas within an IPdu with this bit patter */
    const uint8 ComTxIPduUnusedAreasDefault;
    /* ComFilter evaluates to true */
    const ComTxModeTrue_type ComTxModeTrue ;
    /* ComFilter evaluates to ComTxModeFalse */
    const ComTxModeFalse_type ComTxModeFalse ;
} ComTxIPdu_type;


typedef struct {

    /* The period between successive calls to Com_MainFunctionRx in ms */
    uint16 ComRxTimeBase;

    /* The period between successive calls to Com_MainFunctionTx in ms */
    uint16 ComTxTimeBase;
} ComTimeBase_type;

typedef enum {
    BOOLEAN,
    FLOAT32,
    FLOAT64,
    UINT8,
    UINT16,
    UINT32,
    UINT8_N,
    UINT8_DYN,
    SINT8,
    SINT16,
    SINT32,
    SINT64,
    UINT64
} ComSignalType_type;

/*ComRxDataTimeoutAction */
typedef enum {
    SUBSTITUTE_ACTION,
    REPLACE_ACTION,
    NONE_ACTION
} ComRxDataTimeout_Action;

/* Contains the configuration parameters of the AUTOSAR COM module's signals. */
typedef struct {
    /*  This parameter refers to the position in the I-PDU and not in the shadow buffer.*/
    uint32 ComBitPosition;
    /*  Bit position of update-bit inside I-PDU.
        If this attribute is omitted then there is no update-bit. This setting must be consistently on sender and on receiver side.
        Range: 0..63 for CAN and LIN, 0..511 for CAN FD, 0..2031 for FlexRay, 0..4294967295 for TP.*/
    uint32 ComUpdateBitPosition;
    /* The numerical value used as the ID.
       This ID identifies signals and signal groups in the COM APIs using Com_SignalIdType or Com_SignalGroupIdType parameter respectively. */
    uint16 ComHandleId;
    /* Size in bits, for integer signal types. For ComSignalType UINT8_N and UINT8_DYN
       the size shall be configured by ComSignalLength. For ComSignalTypes FLOAT32 and FLOAT64 the size is already defined by the signal type 
	   and therefore may be omitted. */
    uint8 ComBitSize;
    /*  The AUTOSAR type of the signal. Whether or not the signal is signed or unsigned can be found by examining the value of this attribute.
        This type could also be used to reserved appropriate storage in AUTOSAR COM.*/
    const ComSignalType_type ComSignalType;
    /*  Defines if a write access to this signal can trigger the transmission of the correspon-ding I-PDU.
     *  If the I-PDU is triggered, depends also on the transmission mode of the corresponding I-PDU.*/
    const ComTransferProperty_type ComTransferProperty;
    //string  ComSignalInitValue; that parameter we don't know its datatype

    /* IPDU id of the IPDU that this signal belongs to.
     * This is initialized by Com_Init() and should not be configured.
     */
    const uint16 ComIPduHandleId; // Not in SWS

    void *  ComSignalDataPtr;

    /* notification function. */
    void (*ComNotification) (void);


    /*used to indicate if update bit used in this signal*/
    boolean isUpdateBitUsed ;

    /*Action to be executed if the invalid value of a signal received */
    ComDataInvalidAction_type ComDataInvalidAction ;

    /*Pointer to the array containing the invalid value*/
    uint8 *ComSignalDataInvalidValue;

    /*Pointer to function the notification function */
    void (*ComInvalidNotification) (void);

    /*Pointer to array of uint8 to save the initial value of the signal*/
    uint8 *ComSignalInitValue;

    /*flag to indicate if we used invalid signal value or not*/
    boolean isInvaildSignalUsed ;

    /*flag to indicate that the invalid value is received */
    boolean isInvaildSignalChecked ;
    const boolean IsGroupSignal ;
    /*  This parameter refers First Time Out in ms.*/
    EcucFloatParamDef ComFirstTimeout;

    /*  This parameter refers Time Out in ms.*/
    EcucFloatParamDef ComTimeout;

    /*  This parameter refers Time Out Action.*/
    ComRxDataTimeout_Action ComRxDataTimeoutAction;

    /*Pointer to function the notification function */
    void (*ComTimeoutNotification) (void);

} ComSignal_type;


typedef struct
{
    /* The numerical value used as the ID.
     This ID identifies signals and signal groups in the COM APIs using Com_SignalIdType or Com_SignalGroupIdType parameter respectively. */
    uint16 ComHandleId;
    /*  Bit position of update-bit inside I-PDU.
     If this attribute is omitted then there is no update-bit. This setting must be consistently on sender and on receiver side.
     Range: 0..63 for CAN and LIN, 0..511 for CAN FD, 0..2031 for FlexRay, 0..4294967295 for TP.*/
    uint32 ComUpdateBitPosition;

    /* notification function. */
    void (*ComNotification)(void);
    /*  Defines if a write access to this signal can trigger the transmission of the correspon-ding I-PDU.
     *  If the I-PDU is triggered, depends also on the transmission mode of the corresponding I-PDU.*/
    const ComTransferProperty_type ComTransferProperty;

    void * const ComSignalGroupDataPtr;

    uint16 *GroupSignals;/*not in sws*/

    uint8 number_GroupSignals;/*not in sws*/
    const uint16 ComIPduHandleId; /*Not in SWS*/


} ComSignalGroup_type;
typedef struct {
    /* size of the Pdu in bytes */
    uint8 ComIPduSize; /* violation of the sws container */

    /* sent or received */
    ComIPduDirection_type ComIPduDirection;
    /* The numerical value used as the ID of this I-PDU */
    uint16 ComIPduHandleId ;
    /* Container of transmission related parameters. */
    const ComTxIPdu_type ComTxIPdu;

    ComIPduSignalProcessing_type ComIPduSignalProcessing;

    /* Reference to the actual pdu data storage */
    void *const ComIPduDataPtr;     /* violation of the sws container */

    ComIPduType_type ComIPduType;

    /* References to all signals  contained in this IPDU.
      It probably makes little sense not to define at least one signal or signal group for each IPDU.
     */
    const ComSignal_type * const *ComIPduSignalRef;
    /*References to all signal groups contained in this I-Pdu*/
    const ComSignalGroup_type * const *ComIPduSignalGroupRef;
} ComIPdu_type;



typedef struct
{
    /*  This parameter refers to the position in the I-PDU and not in the shadow buffer.*/
    uint32 ComBitPosition;
    /* The numerical value used as the ID.
     This ID identifies signals and signal groups in the COM APIs using Com_SignalIdType or Com_SignalGroupIdType parameter respectively. */
    uint16 ComHandleId;
    /* Size in bits, for integer signal types. For ComSignalType UINT8_N and UINT8_DYN
     the size shall be configured by ComSignalLength. For ComSignalTypes FLOAT32 and FLOAT64 the size is already defined by the signal type
     and therefore may be omitted. */
    uint8 ComBitSize;
    /*  The AUTOSAR type of the signal. Whether or not the signal is signed or unsigned can be found by examining the value of this attribute.
     This type could also be used to reserved appropriate storage in AUTOSAR COM.*/
    const ComSignalType_type ComSignalType;
    /*  Defines if a write access to this signal can trigger the transmission of the correspon-ding I-PDU.
     *  If the I-PDU is triggered, depends also on the transmission mode of the corresponding I-PDU.*/
    const ComTransferProperty_type ComTransferProperty;
    void * const ComSignalDataPtr;
    const uint16 ComIPduHandleId; /*Not in SWS*/
    const boolean IsGroupSignal ;

} ComGroupSignal_type;


typedef struct{
    const SignalType Type;
    uint16 Id;
}SignalAndGroupSignal_type;


/* contains the configuration parameters and sub containers of the AUTOSAR COM module */
typedef struct
{

    const ComTimeBase_type ComTimeBase;

    /* IPDU definitions */
    const ComIPdu_type * ComIPdu;

    /* Signal definitions */
    const ComSignal_type *ComSignal;
    /*Signal Group definitions*/
    const ComSignalGroup_type *ComSignalGroup;
    /*Group Signal definitions*/
    const ComGroupSignal_type *ComGroupSignal;
    const SignalAndGroupSignal_type *SignalAndGroupSignal;

} ComConfig_type;

typedef struct {
    boolean ComRetryFailedTransmitRequests ;
} ComGeneral_type;


#endif
