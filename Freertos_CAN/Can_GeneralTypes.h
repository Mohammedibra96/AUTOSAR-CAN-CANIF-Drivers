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

#include <Can_Cfg.h>
#include <ComStack_Types.h>
#include <Mcu_Cfg.h>
#include "stdbool.h"



/************************************************************    MACROS   *************************************************************/
#define   BOFF                         0xE0U
#define   EPASS                        0x60U
#define   EWARN                        0x40U




#define   INTERRUPT                    0U
#define   MIXED                        1U
#define   POLLING                      2U


#define     STANDARD      (uint8_t)0
#define     EXTENDED      (uint8_t)1

#define     CONFIRMED         (uint8_t)0
#define     UNCONFIRMED       (uint8_t)1

/*   Handle Type */
#define     FULL      (uint8_t)0
#define     BASIC     (uint8_t)1

/*  Can_ObjectTypeValues  */
#define TRANSMIT  1
#define RECEIVE   0



/*  StateTransition  */
#define     CAN_CS_UNINIT    (uint8_t)0
#define     CAN_CS_STARTED     (uint8_t)1
#define     CAN_CS_STOPPED      (uint8_t)2
#define     CAN_CS_SLEEP     (uint8_t)3
#define     CAN_CS_WAKEUP    (uint8_t)4

/******************************************************     Standard Types needed in the can driver      ***********************************************************************/

typedef uint8_t Can_ControllerStateType;
typedef uint8_t Can_ConfigType;

/*  [SWS_Can_00416]  */
typedef uint32_t Can_IdType;

typedef uint8_t confirmation_t    ;
typedef uint16_t Can_HwHandleType ;
typedef uint8_t CanObjectType_t ;

typedef uint8_t CanHandleType_t     ;
typedef uint8_t Can_StateTransitionType;
/*  [SWS_Can_91003]  */
typedef enum{
    CAN_ERRORSTATE_ACTIVE,
    CAN_ERRORSTATE_PASSIVE,
    CAN_ERRORSTATE_BUSOFF
}Can_ErrorStateType;




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
    CanHandleType_t CanHandleType;

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
    CanObjectType_t CanObjectType;
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
    /*Specifies (together with the filter mask) the identifiers range that passes the hardware filter.*/
    uint32_t CanHwFilterCode ;

    uint8_t FilterStartRef         ;
    uint8_t NumberOfFilter         ;

}CanHardwareObject_s;



#endif /* INCLUDES_CAN_GENERALTYPES_H_ */
