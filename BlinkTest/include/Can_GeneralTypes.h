/*
 * Can_GeneralTypes.h
 *
 *  Created on: June 4th, 2020
 *  Author: ITI Students Mahmoud Gamal & Mohammed Ibrahim
 *  Version: 1.0
 */

/*[SWS_CAN_00487]⌈ The types specified in Can_GeneralTypes shall be declared in Can_GeneralTypes.h */
/*[SWS_Can_00439]⌈The content of Can_GeneralTypes.h shall be protected by a CAN_GENERAL_TYPES define. */
/*[SWS_Can_00440]⌈If different CAN drivers are used, only one instance of this file has to be included in the source tree.
For implementation all Can_GeneralTypes.h related types in the documents mentioned before shall be considered.*/

#ifndef CAN_GENERAL_TYPES
#define CAN_GENERAL_TYPES

/*
Can_GeneralTypes.h shall contain all types and constants that
are shared among the AUTOSAR CAN modules Can, CanIf and CanTrcv.
 */

#include "include/Can_Cfg.h"
#include "include/Mcu_Cfg.h"
#include "include/ComStack_Types.h"
#include "stdbool.h"



/************************************************************    MACROS   *************************************************************/
#define   BOFF                         0xE0U
#define   EPASS                        0x60U
#define   EWARN                        0x40U

#define   CAN_E_PARAM_POINTER          1U
#define   CAN_E_PARAM_HANDLE           2U
#define   CAN_E_PARAM_DATA_LENGTH      3U
#define   CAN_E_PARAM_CONTROLLER       4U
#define   CAN_E_UNINIT                 5U
#define   CAN_E_TRANSITION             6U
#define   CAN_E_PARAM_BAUDRATE         7U
#define   CAN_E_ICOM_CONFIG_INVALID    8U
#define   CAN_E_INIT_FAILED            9U

#define   INTERRUPT                    0U
#define   MIXED                        1U
#define   POLLING                      2U


#define     STANDARD      0
#define     EXTENDED      1

#define     CONFIRMED         1
#define     UNCONFIRMED       0

/*   Handle Type */
#define     FULL      0               
#define     BASIC     1

/*  Can_ObjectTypeValues  */
#define CAN_OBJECT_TYPE_TRANSMIT  1
#define CAN_OBJECT_TYPE_RECEIVE   0


/*  StateTransition  */
#define     CAN_T_START     0
#define     CAN_T_STOP      1
#define     CAN_T_SLEEP     2
#define     CAN_T_WAKEUP    3  

/******************************************************     Standard Types needed in the can driver      ***********************************************************************/

/***************************
TODO
add _t

- GO with the Tx sequence and check if we need any further Look Up tables

********************************/


typedef uint8_t Can_ConfigType;

/*  [SWS_Can_00416]  */
typedef uint32_t Can_IdType;

typedef uint8_t confirmation_t    ;
typedef uint16_t Can_HwHandleType ;
typedef uint8_t CanObjectType ;

typedef uint8_t CanHandleType     ;
typedef uint8_t Can_StateTransitionType;
/*  [SWS_Can_91003]  */
typedef enum{
  CAN_ERRORSTATE_ACTIVE,
  CAN_ERRORSTATE_PASSIVE,
  CAN_ERRORSTATE_BUSOFF
}Can_ErrorStateType;

/*  [SWS_Can_91013]  */
typedef enum {
  CAN_CS_UNINIT  =  0x00,
  CAN_CS_STARTED =  0x01,
  CAN_CS_STOPPED =  0x02,
  CAN_CS_SLEEP   =  0x03
}Can_ControllerStateType;


/*  [SWS_Can_00039]:
 *  Overlayed return value of Std_ReturnType for CAN driver API Can_Write()
 */


#define    CAN_BUSY   0x2



/*    keep this as an enum (because this is a state machine and you need an error if th user defines any values 
      that are not)    */
typedef enum{
  CAN_UNINIT,
  CAN_READY
}Can_StateType;

/*  [SWS_Can_00415]  */
typedef struct 
{
  PduIdType     swPduHandle;
  uint8_t       length;
  Can_IdType    id;
  uint8_t*      sdu;
} Can_PduType;


/* [SWS_Can_00429]   */
typedef struct 
{
  Can_IdType        CanId;          /*uint32_t*/
  Can_HwHandleType  Ho;             /*uint32_t*/
  uint8_t           controllerlId;

}Can_HwType;


/*********************************************  NOT IN THE SWS  **************************************************************/

/*
 * CanGenreral: contains the parameters related each CAN Driver Unit.
 * NOTE: this struct has been removed and instead we are using macros that you can find in Can_Cfg.h
 */


typedef struct 
{
 confirmation_t   Confirmation ;
 uint8_t        pduId        ;
 uint8_t        HwObject     ;
}Can_MailBoxLookUpTables_s   ;

/*****************************************  CAN CONFIGURATION TYPES ******************************************************/


/***************************
TODO
add _s

********************************/

/* This container "CanController" contains the configuration /
 * parameters of the CAN controller(s)./
 * This container is a Sub container of "CanConfigSet" container with /
 * upperMultiplicity = * ,lowerMultiplicity = 1 */
typedef struct 
{

/* This parameter provides the controller ID which is unique in a given CAN Driver./
 * The value for this parameter starts with 0 and continue without any gaps./
 * The Range of CanControllerId is 0 .. 255, with Multiplicity 1.
 */
 uint8_t CanControllerId;

/* This parameter specifies the CAN controller base address.
 * The Range of CanControllerBaseAddress is 0 .. 4294967295, with Multiplicity 1
 */
 uint32_t CanControllerBaseAddress;


 /*
  * THIS IS NOT A PART OF THE SWS
  * This parameter should refernece the INT_CAN0 or INT_CAN1
  */
 uint32_t CanInterruptId;

/* Reference to baud rate configuration container configured for the Can Controller.
 * The Multiplicity is 1.
 */
 uint16_t  CanControllerDefaultBaudrate;

}CanController_s;




/* This container contains bit timing related configuration/
 * parameters of the CAN controller(s)./
 * This container is a Sub container of "CanController" container with/
 * upperMultiplicity = * ,lowerMultiplicity = 1/
 */
// TODO: REMOVE CanControllerBaudRateConfigID

typedef struct CanControllerBaudrateConfig
{

/*Specifies the baud rate of the controller in kbps.
 *Range 0 .. 2000, Multiplicity 1
 */
 uint32_t CanControllerBaudRate;

 /*Uniquely identifies a specific baud rate configuration. This ID is used by SetBaudrate API.
  *Range 0 .. 65535,  Multiplicity 1
  *Default value = 0 */

 uint16_t CanControllerBaudRateConfigID;

 /*Specifies propagation delay in time quantas.
  *Range 0 .. 255, Multiplicity 1 */

 uint8_t CanControllerPropSeg;

 /*Specifies phase segment 1 in time quantas.
  *Range 0 .. 255, Multiplicity 1 */

 uint8_t CanControllerSeg1;

 /*Specifies phase segment 2 in time quantas.
  *Range 0 .. 255, Multiplicity 1 */

 uint8_t CanControllerSeg2;

/*Specifies the synchronization jump width for the controller in time quanta.
 *Range 0 .. 255, Multiplicity 1 */

 uint8_t CanControllerSyncJumpWidth;
}CanControllerBaudrateConfig_s;



/*
 * This container contains the configuration (parameters) of CAN Hardware Objects.
 */
typedef struct CanHardwareObject
{
/*
 * Specifies the type (Full-CAN or Basic-CAN) of a hardware object.
 */
    CanHandleType CanHandleType;

/*
 * Number of hardware objects used to implement one HOH.
 *  In case of a HRH this parameter defines the number of elements in the hardware FIFO or the number of shadow buffers,
 *  in case of a HTH it defines the number of hardware objects used for multiplexed transmission or
 *  for a hardware FIFO used by a FullCAN HTH.
 *
 */
 uint8_t CanHwObjectCount;

/*
 * Specifies whether the IdValue is of type
 *  standard identifier
 *  extended identifier
 *   mixed mode
 */
 Can_IdType CanIdType;

/*
 * Holds the handle ID of HRH or HTH. The value of this parameter is unique in a given CAN Driver,
 * and it should start with 0 and continue without any gaps.
 * The HRH and HTH Ids share a common ID range.
 * Example: HRH0-0, HRH1-1, HTH0-2, HTH1-3
 *
 */
 uint16_t CanMailBoxStart;

/*
 * Specifies if the HardwareObject is used as Transmit or as Receive object
 *
 */
 CanObjectType CanObjectType;
/*
 * Reference to CAN Controller to which the HOH is associated to.
 *
 */
 uint8_t CanControllerRef ;

/*
 * Specifies (together with the filter mask) the identifiers range that passes the hardware filter.
 * (This is the index of the filter in CanHwFilterMask)
 * this parameter was initially in the struct CanHwFilter, but it was removed for a more simple and efficient implementation
 */


 /***********************
  EL HABD DA BETA3 HEMA ANA MALEESH DA3WA :3

 **********************************/
 uint32_t CanHwFilterCode[NUM_FILTERS] ;

}CanHardwareObject_s;




#endif /* INCLUDES_CAN_GENERALTYPES_H_ */
