#include "include/Can_GeneralTypes.h"
#include "include/Can.h"
#include "include/Can_Cfg.h"






CONST(CanControllerBaudrateConfig_s ,AUTOMATIC )  CanControllerBaudrateConfig[MAX_NUM_OF_BAUDRATES ] = {
                                                                                     {
                                                                                      .CanControllerBaudRate = 500U,
                                                                                      .CanControllerPropSeg = 2U,
                                                                                      .CanControllerSeg1 = 6U,
                                                                                      .CanControllerSeg2 = 7U,
                                                                                      .CanControllerSyncJumpWidth = 4U,
                                                                                      .CanControllerBaudRateConfigID = 0U,
                                                                                     },
                                                                                     {
                                                                                      .CanControllerBaudRate = 500U,
                                                                                      .CanControllerPropSeg = 2U,
                                                                                      .CanControllerSeg1 = 3U,
                                                                                      .CanControllerSeg2 = 4U,
                                                                                      .CanControllerSyncJumpWidth = 2U,
                                                                                      .CanControllerBaudRateConfigID = 1U,
                                                                                     },
                                                                                     {
                                                                                      .CanControllerBaudRate = 400U,
                                                                                      .CanControllerPropSeg = 2U,
                                                                                      .CanControllerSeg1 = 6U,
                                                                                      .CanControllerSeg2 = 7U,
                                                                                      .CanControllerSyncJumpWidth = 2U,
                                                                                      .CanControllerBaudRateConfigID = 2U,
                                                                                     },
                                                                                     {
                                                                                      .CanControllerBaudRate = 400U,
                                                                                      .CanControllerPropSeg = 2U,
                                                                                      .CanControllerSeg1 = 6U,
                                                                                      .CanControllerSeg2 = 7U,
                                                                                      .CanControllerSyncJumpWidth = 3U,
                                                                                      .CanControllerBaudRateConfigID = 3U,
                                                                                     },
                                                                                     {
                                                                                      .CanControllerBaudRate = 150U,
                                                                                      .CanControllerPropSeg = 2U,
                                                                                      .CanControllerSeg1 = 7U,
                                                                                      .CanControllerSeg2 = 8U,
                                                                                      .CanControllerSyncJumpWidth = 2U,
                                                                                      .CanControllerBaudRateConfigID = 4U,
                                                                                     },
                                                                                     {
                                                                                      .CanControllerBaudRate = 100U,
                                                                                      .CanControllerPropSeg = 2U,
                                                                                      .CanControllerSeg1 = 6U,
                                                                                      .CanControllerSeg2 = 7U,
                                                                                      .CanControllerSyncJumpWidth = 4U,
                                                                                      .CanControllerBaudRateConfigID = 5U,
                                                                                     }
};


/*
 * Describes a mask for hardware-based filtering of CAN identifiers./
 * The CAN identifiers of incoming messages are masked with the appropriate/
 * CanFilterMaskValue.Bits holding a 0 mean don't care,/
 * i.e. do not compare the message's identifier in the respective bit position./
 * The mask shall be build by filling with leading 0./
 * The In case of CanIdType EXTENDED or MIXED a 29 bit mask shall
 */
CONST( uint32_t ,AUTOMATIC ) CanHwFilterMask[MAX_NUM_FILTERS]  = { 0 };

/*
 * This container contains the configuration parameters of the CAN controller(s).
 */
VAR(CanController_s ,AUTOMATIC ) CanController[MAX_NUM_OF_CAN_CONTROLLERS] =
{
 {
  .CanControllerId              = 0U,
  .CanControllerBaseAddress     = 0x40040000U,
  .CanControllerDefaultBaudrate = 0
 },
 {
  .CanControllerId              = 1U,
  .CanControllerBaseAddress     = 0x40041000U,
  .CanControllerDefaultBaudrate = 2
 }
};

/*
 * This container contains the configuration (parameters) of CAN Hardware Objects.
 */
VAR(CanHardwareObject_s ,AUTOMATIC ) CanHardwareObject[ MAX_NUM_OF_HO ] =
{
 {
  .CanHandleType    = FULL,
  .CanHwObjectCount = 1,
  .CanIdType        = STANDARD,
  .CanObjectType    = TRANSMIT,
  .CanControllerRef = 0,
  .CanHwFilterCode  ={
                      0
  },
 },
 {
  .CanHandleType    = FULL,
  .CanObjectType    = TRANSMIT,
  .CanIdType        = STANDARD,
  .CanHwObjectCount = 3,
  .CanControllerRef = 0,
  .CanHwFilterCode = {
                      0
  },
 },
 {
  .CanHandleType    = BASIC,
  .CanObjectType    = RECEIVE,
  .CanIdType        = STANDARD,
  .CanHwObjectCount = 7,
  .CanControllerRef = 0,
  .CanHwFilterCode  =
  {
   0
  }
 }
};




