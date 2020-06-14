/*
 * Static_Can_Cfg.h
 *
 *  Created on: May 3, 2018
 *      Author: Ahmed Usama Khalifa
 */
#ifndef INCLUDES_CAN_CFG_H_
#define INCLUDES_CAN_CFG_H_



#define  MAX_NUM_OF_HO     30
#define MAX_NUM_CONTROLLER  2


#define  MAX_NUM_FILTERS                  1u
#define  Max_CanHwObjectCount             1U
#define  MAX_NUM_OF_CAN_CONTROLLERS       2U


#define False 0U
#define True  1U



/*************  All these macros are parameters in the CanGeneral struct, but have been moved them out because we are using precombiled configurations   **************/
#define  CAN_GENERAL_CAN_VERSION_INFO_API                   False
#define  CAN_GENERAL_CAN_INDEX                              0U
#define  CAN_GENERAL_CAN_MAIN_FUNCTION_MODE_PERIOD          0U
#define  CAN_GENERAL_CAN_SET_BAUDRATE_API                   True
#define  CAN_GENERAL_CAN_DEV_ERROR_DETECT                   TRUE
#define  CAN_GENERAL_CAN_TIMEOUT_DURATION                   0.5f


#define  CAN_CONTROLLER_ACTIVATION                          True
#define  CAN_CONTROLLER_CAN_TX_PROCESSING                   POLLING
#define  CAN_CONTROLLER_CAN_RX_PROCESSING                   POLLING
#define  CAN_SET_BAUDRATE_API                               TRUE
#define  MAX_NUM_OF_BAUDRATES  6U









#endif /* INCLUDES_CAN_CFG_H_ */

