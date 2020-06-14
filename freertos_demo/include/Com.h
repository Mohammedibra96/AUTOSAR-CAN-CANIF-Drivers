
 /*******************************************************
 *
 * File Name: Com.h
 *
 * Author: AUTOSAR COM Team 
 * 
 * Date Created: 6 March 2019	
 * 
 * Version	: 01	
 * 
 ********************************************************/
 
#ifndef COM_H_
#define COM_H_


#include "ComStack_Types.h"
#include "Com_Cfg.h"
#include "Com_helper.h"
#include "Com_Cbk.h"
#include "PduR_Com.h"
#include "PduR.h"

//#include "Com_Types.h"  /* to be changed to ComStack_Types.h */

/************************************************************************
 *                          Preprocessor                                *
 ************************************************************************/

#define COM_BUSY 0x81



/************************************************************************
 *                      Functions Prototypes                            *
 ************************************************************************/

/*initializes internal and external interfaces and variables of the COM module */
void Com_Init( const ComConfig_type* config);
 
/* Processing of the AUTOSAR COM module's receive activities (PDU To Signal) */
void Com_MainFunctionRx(void);

/* Processing of the AUTOSAR COM module's transmission activities (Signal To PDU)*/
void Com_MainFunctionTx(void);

/* Updates the signal object identified by SignalId with the signal referenced by the SignalDataPtr parameter */
uint8 Com_SendSignal( Com_SignalIdType SignalId, const void* SignalDataPtr );
uint8 Com_SendSignalv2( Com_SignalGroupIdType SignalId, const void* SignalDataPtr );

/* Copies the data of the signal identified by SignalId to the location specified by SignalDataPtr */
uint8 Com_ReceiveSignal( Com_SignalIdType SignalId, void* SignalDataPtr );


uint8 Com_InvalidateSignal(Com_SignalIdType SignalId);

/* the I-PDU with the given ID is triggered for transmission */
Std_ReturnType Com_TriggerIPDUSend( PduIdType PduId );

/* Cbk function, indation from the PduRouter of recieving Pdu */
//process signals if the signal processing mode is IMMEDIATE -> notify the signal callback , else -> mark signal as updated
static void Com_RxProcessSignals(PduIdType ComRxPduId);

uint8 SendSignal( Com_SignalIdType SignalId, const void* SignalDataPtr );
uint8 SendGroupSignal( Com_SignalGroupIdType SignalId, const void* SignalDataPtr );
uint8 Com_SendSignal(Com_SignalGeneral SignalId,const void *SignalDataPtr);
/*The service Com_SendSignalGroup copies the content of the associated shadow buffer to the associated I-PDU.*/
uint8 Com_SendSignalGroup(Com_SignalGroupIdType SignalGroupId);
uint8 Com_RecieveSignalGroup  ( Com_SignalGroupIdType SignalGroupId );

/*The service Com_DLN_Init initializes Deadline Monitoring timers.*/
void Com_DLN_InitRX(void);

/*The service Com_DLN_Init Processes Deadline Monitoring timers.*/
void Com_DLN_ProcessingRX( PduIdType PduId , boolean pduUpdated );

/*The service Com_DLN_Init Resets Deadline Monitoring timers.*/
void Com_DLN_Reset( PduIdType PduId );


#endif

