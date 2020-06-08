/*
 * Static_Can_Cfg.h
 *
 *  Created on: May 3, 2018
 *      Author: Ahmed Usama Khalifa
 */
#ifndef INCLUDES_CAN_CFG_H_
#define INCLUDES_CAN_CFG_H_



#define False 0U
#define True  1U


/*************  All these macros are parameters in the CanGeneral struct, but have been moved them out because we are using precombiled configurations   **************/
#define  CAN_GENERAL_CAN_VERSION_INFO_API                   False
#define  CAN_GENERAL_CAN_INDEX                              0U
#define  CAN_GENERAL_CAN_MAIN_FUNCTION_MODE_PERIOD          0U
#define  CAN_GENERAL_CAN_SET_BAUDRATE_API                   True
#define  CAN_GENERAL_CAN_DEV_ERROR_DETECT                   False
#define  CAN_GENERAL_CAN_TIMEOUT_DURATION                   0.5f


/*************  All these macros are parameters in the CanGeneral struct, but have been moved them out because we are using precombiled configurations   **************/
#define  CAN_CONTROLLER_ACTIVATION


#define UserCANCfg \
{.CanConfigSet.CanController =\
    {\
{\
.CanControllerId = 0U,\
.CanControllerActivation = True,\
.CanControllerBaseAddress = 0x40040000U,\
.CanCpuClockRef=&McuPerClockConfigData,\
.CanControllerDefaultBaudrate = &CanContainer.CanConfigSet.CanController[0U].CanControllerBaudrateConfig[0U],\
.CanControllerBaudrateConfig=\
{\
 {\
.CanControllerBaudRate = 500U,\
.CanControllerPropSeg = 2U,\
.CanControllerSeg1 = 6U,\
.CanControllerSeg2 = 7U,\
.CanControllerSyncJumpWidth = 4U,\
.CanControllerBaudRateConfigID = 0U,\
        },\
 {\
.CanControllerBaudRate = 500U,\
.CanControllerPropSeg = 2U,\
.CanControllerSeg1 = 3U,\
.CanControllerSeg2 = 4U,\
.CanControllerSyncJumpWidth = 2U,\
.CanControllerBaudRateConfigID = 1U,\
        },\
 {\
.CanControllerBaudRate = 400U,\
.CanControllerPropSeg = 2U,\
.CanControllerSeg1 = 6U,\
.CanControllerSeg2 = 7U,\
.CanControllerSyncJumpWidth = 2U,\
.CanControllerBaudRateConfigID = 2U,\
        },\
 {\
.CanControllerBaudRate = 400U,\
.CanControllerPropSeg = 2U,\
.CanControllerSeg1 = 6U,\
.CanControllerSeg2 = 7U,\
.CanControllerSyncJumpWidth = 3U,\
.CanControllerBaudRateConfigID = 3U,\
        },\
 {\
.CanControllerBaudRate = 150U,\
.CanControllerPropSeg = 2U,\
.CanControllerSeg1 = 7U,\
.CanControllerSeg2 = 8U,\
.CanControllerSyncJumpWidth = 2U,\
.CanControllerBaudRateConfigID = 4U,\
        },\
 {\
.CanControllerBaudRate = 100U,\
.CanControllerPropSeg = 2U,\
.CanControllerSeg1 = 6U,\
.CanControllerSeg2 = 7U,\
.CanControllerSyncJumpWidth = 4U,\
.CanControllerBaudRateConfigID = 5U,\
        }\
       }\
      },\
{\
.CanControllerId = 1U,\
.CanControllerActivation = True,\
.CanControllerBaseAddress = 0x40041000U,\
.CanCpuClockRef=&McuPerClockConfigData,\
.CanControllerDefaultBaudrate = &CanContainer.CanConfigSet.CanController[1U].CanControllerBaudrateConfig[0U],\
.CanControllerBaudrateConfig=\
{\
 {\
.CanControllerBaudRate = 500U,\
.CanControllerPropSeg = 2U,\
.CanControllerSeg1 = 6U,\
.CanControllerSeg2 = 7U,\
.CanControllerSyncJumpWidth = 4U,\
.CanControllerBaudRateConfigID = 0U,\
        }\
       }\
 }\
 }\
};

#define  CanTxProcessing0 0U
#define  CanRxProcessing0 0U
#define  CanBusoffProcessing0 0U
#define  CanTxProcessing1 0U
#define  CanRxProcessing1 0U
#define  CanBusoffProcessing1 0U

#define hohMap \
{.CanConfigSet.CanHardwareObject =\
    {\
        {\
           .CanHandleType = FULL,\
           .CanObjectType = transmit,\
           .CanIdType = STANDARD,\
           .CanHwObjectCount=1,\
           .CanObjectId=0,\
           .CanTriggerTransmitEnable = FALSE,\
           .CanFdPaddingValue = 0U,\
           .CanControllerRef = &CanContainer.CanConfigSet.CanController[0],\
        },\
             {\
           .CanHandleType = FULL,\
           .CanObjectType = transmit,\
           .CanIdType = STANDARD,\
           .CanHwObjectCount=1,\
           .CanObjectId=1,\
           .CanTriggerTransmitEnable = FALSE,\
           .CanFdPaddingValue = 0U,\
           .CanControllerRef = &CanContainer.CanConfigSet.CanController[0],\
        },\
        {\
           .CanHandleType = FULL,\
           .CanObjectType = receive,\
           .CanIdType = STANDARD,\
           .CanHwObjectCount=1,\
           .CanObjectId= 0,\
           .CanTriggerTransmitEnable = FALSE,\
           .CanFdPaddingValue = 0U,\
           .CanControllerRef = &CanContainer.CanConfigSet.CanController[0],\
           .CanHwFilter=\
           {\
             .CanHwFilterMask=0xFFFFFFFF,\
             .CanHwFilterCode=0x0,\
          }\
        }\
    }\
};


#define  NUM_OF_HTH  2U
#define  NUM_OF_HRH  1U
#define  NUM_OF_HOH  3U
#define  Max_CanHwObjectCount  1U
#define  Max_num_of_baudrates  6U
#define  NUM_OF_CAN_CONTROLLERS  2U

#define SYSCTL_PERIPH \
    {\
        {.AHBClocksEnable = 0xf0003400U,\
         .APB1ClocksEnable = 0U,\
         .APB2ClocksEnable = 0U\
        },\
        {.AHBClocksEnable = 0xf0003401U,\
         .APB1ClocksEnable = 0U,\
         .APB2ClocksEnable = 0U\
        }\
    };

#endif /* INCLUDES_CAN_CFG_H_ */

