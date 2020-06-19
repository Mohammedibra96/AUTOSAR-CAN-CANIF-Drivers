#include <Can.h>
#include <Can_Cfg.h>
#include <Can_GeneralTypes.h>






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
                                                                                                         .CanControllerBaudRate = 200U,
                                                                                                         .CanControllerPropSeg = 2U,
                                                                                                         .CanControllerSeg1 = 3U,
                                                                                                         .CanControllerSeg2 = 4U,
                                                                                                         .CanControllerSyncJumpWidth = 2U,
                                                                                                         .CanControllerBaudRateConfigID = 1U,
                                                                                                        },
                                                                                                        {
                                                                                                         .CanControllerBaudRate = 100U,
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
  .CanHandleType    = BASIC,
  .CanHwObjectCount = 3,
  .CanIdType        = STANDARD,
  .CanObjectType    = TRANSMIT,
  .CanControllerRef = 0,
  .CanHwFilterCode  = 0,
  .FilterStartRef  = 0 ,
  .NumberOfFilter  = 2 ,
 },

 {
  .CanHandleType    = FULL,
  .CanHwObjectCount = 1,
  .CanIdType        = STANDARD,
  .CanObjectType    = TRANSMIT,
  .CanControllerRef = 0,
  .CanHwFilterCode  = 0,
  .FilterStartRef  = 0 ,
  .NumberOfFilter  = 2 ,
 },

 {
  .CanHandleType    = BASIC,
  .CanHwObjectCount = 5,
  .CanIdType        = STANDARD,
  .CanObjectType    = TRANSMIT,
  .CanControllerRef = 0,
  .CanHwFilterCode  = 0,
  .FilterStartRef  = 0 ,
  .NumberOfFilter  = 2 ,
 },

 {
  .CanHandleType    = FULL,
  .CanHwObjectCount = 1,
  .CanIdType        = STANDARD,
  .CanObjectType    = TRANSMIT,
  .CanControllerRef = 0,
  .CanHwFilterCode  = 0,
  .FilterStartRef  = 0 ,
  .NumberOfFilter  = 2 ,
 },
 {
  .CanHandleType    = BASIC,
  .CanHwObjectCount = 3,
  .CanIdType        = STANDARD,
  .CanObjectType    = RECEIVE,
  .CanControllerRef = 0,
  .CanHwFilterCode  = 0,
  .FilterStartRef  = 0 ,
  .NumberOfFilter  = 2 ,
 },

 {
  .CanHandleType    = FULL,
  .CanHwObjectCount = 1,
  .CanIdType        = STANDARD,
  .CanObjectType    = RECEIVE,
  .CanControllerRef = 0,
  .CanHwFilterCode  = 0,
  .FilterStartRef  = 0 ,
  .NumberOfFilter  = 2 ,
 },

 {
  .CanHandleType    = BASIC,
  .CanHwObjectCount = 5,
  .CanIdType        = STANDARD,
  .CanObjectType    = RECEIVE,
  .CanControllerRef = 0,
  .CanHwFilterCode  = 0,
  .FilterStartRef  = 0 ,
  .NumberOfFilter  = 2 ,
 },

 {
  .CanHandleType    = FULL,
  .CanHwObjectCount = 3,
  .CanIdType        = STANDARD,
  .CanObjectType    = RECEIVE,
  .CanControllerRef = 0,
  .CanHwFilterCode  = 0,
  .FilterStartRef  = 0 ,
  .NumberOfFilter  = 2 ,
 },


};

VAR(uint32_t,AUTOMATIC) CanHwFilterCfg[MAX_NUM_FILTERS] ={
                                                          0x0,
                                                          0x0,
                                                          0x0,
                                                          0x0
};



