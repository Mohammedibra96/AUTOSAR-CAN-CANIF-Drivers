/*TODO:Check the mailbox exceed 32 and report custom det and ignore the mailbox above 32 Can_Init                                */
/*TODO:check the full can and ignore the mailboxes that above 1 Can_Init                                                          */
/*TODO:check the cfg can controller  if zero or one or both  add var to contains the cnfg can controller */
/*******************************************************************************************
 *                                                                                          *
 * File Name   : CanAUTO.c                                                                  *
 *                                                                                          *
 * Author      : ITI AUTOSAR (Mohamed Ibrahem, Mahmoud Gamal )                              *
 *                                                                                          *
 * Platform    : TivaC                                                                      *
 *                                                                                          *
 * Date        : 17 Jun 2020                                                                *
 *                                                                                          *
 * Version     : 1.1.0                                                                      *
 *                                                                                          *
 * Description : specifies the AUTOSAR communication stack type header file Release 4.3.1   *
 *                                                                                          *
 ********************************************************************************************/

/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************includes*********************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/*TODO:remove include/                                                                                                          */

#include "include/Can.h"
#include "include/CanIf_Cbk.h"
#include"include/Can_interrupt.h"
/*TODO:remove include/PortingHW.h */
#include "include/PortingHW.h"
/*TODO: Delay(1ms)or 2ms   how the can controller get the status of the controller */
#include"include/MCU.h"
#include"include/Det.h"
/*TODO:Check this misra*/
#pragma RESET_MISRA("all")



/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/*************************************************** MACROS *********************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/*TODO:create file Det_Can.h and add these Ids   */
/* Service Ids                                                                                                                   */
#define CAN_INIT_SID                           (uint8_t)0x00
#define CAN_MAINFUNCTION_WRITE_SID             (uint8_t)0x01
#define CAN_INIT_CONTROLLER_SID                (uint8_t)0x02
#define CAN_SET_CONTROLLER_MODE_SID            (uint8_t)0x03
#define CAN_SET_BAUDRATE_SID                   (uint8_t)0x0F
#define CAN_GET_CONTROLLER_MODE_SID            (uint8_t)0x12
#define CAN_DISABLE_CONTROLLER_INTERRUPTS_SID  (uint8_t)0x04
#define CAN_ENABLE_CONTROLLER_INTERRUPTS_SID   (uint8_t)0x05
#define CAN_WRITE_SID                          (uint8_t)0x06
#define CAN_MAIN_FUNCTION_READ_SID             (uint8_t)0x08
#define CAN_DEINIT_SID                         (uint8_t)0x10
#define CAN_GET_CONTROLLER_ERROR_STATUS_SID    (uint8_t)0x11
#define CAN_INSTANCE_ID_0                      (uint8_t)CAN_CONTROLLER_ZERO
#define CAN_INSTANCE_ID_1                      (uint8_t)CAN_CONTROLLER_ONE
#define CAN_MODULE_ID                          (uint16_t)MODULE_ID_CAN

/*TODO:Rename this macro TO IS_MB_BUSY()*/
#define MESSAGE_WAITING_MASK  0x100
#define IF1CMSK_MASK          0x80
/*CAN_CODE this macro for Can code memory section  */
#define  CAN_CODE                1
/*TODO:Check IS_VALID_MB(MB_NUM) */
#define  FIRST_MAIL_BOX          1
#define  LAST_MAIL_BOX           32
/*TODO:Make this MAcro like function IS_*/
#define  CAN_CTL_R_INIT_BIT      1U
/*TODO:Remove this macro with E_OK   */
#define  NO_ERROR                0U
/*Replace this using meaning macro FIRST_MAIL_BOX   */
#define  START_INDEX             0U
/*IS_VALID_DATA_RANGE(Size)*/
#define  MIN_DATA_SIZE           1U
#define  MAX_DATA_LENGTH         8U

#define  MAX_NUM_OF_MAILBOXES    32U
/*IS_INTERRUPT_ENABLED()*/
#define  INTERRUPT_ENABLE        1U
#define  INTERRUPT_DISABLE       0U
/*Enum mode status*/
#define MODE_CHANGED             1U
#define MODE_NOT_CHANGED         0U
#define NORMAL_MODE 0U
#define INITIALIZE_STARTED_MODE 1U

/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/*************************************************** Global Variables ***********************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/*TODO:Add lcfg.h  and add these variable there */

/* 
 *This container contains bit timing related configuration parameters of the CAN /
 * controller(s).The Multiplicity is 1..many /
 */
extern CONST( CanControllerBaudrateConfig_s, AUTOMATIC ) CanControllerBaudrateConfig[MAX_NUM_OF_BAUDRATES ]  ;
/*
 * Describes a mask for hardware-based filtering of CAN identifiers./
 * The CAN identifiers of incoming messages are masked with the appropriate/
 * CanFilterMaskValue.Bits holding a 0 mean don't care,/
 * i.e. do not compare the message's identifier in the respective bit position./
 * The mask shall be build by filling with leading 0./
 * The In case of CanIdType EXTENDED or MIXED a 29 bit mask shall
 */


/*
 * This container contains the configuration parameters of the CAN controller(s).
 */
extern VAR(CanController_s,AUTOMATIC) CanController[MAX_NUM_OF_CAN_CONTROLLERS]  ;
/*
 * This container contains the configuration (parameters) of CAN Hardware Objects.
 */
extern VAR(CanHardwareObject_s,AUTOMATIC) CanHardwareObject[ MAX_NUM_OF_HO ];

/*TODO:Comment all global variables */
LOCAL Can_MailBoxLookUpTables_s Can_MailBoxLookUpTables[MAX_NUM_OF_CAN_CONTROLLERS][MAX_NUM_OF_MAILBOXES];

LOCAL VAR(uint8_t,AUTOMATIC) state_transition_flag[2]={MODE_NOT_CHANGED,MODE_NOT_CHANGED}                ;

LOCAL Can_ControllerStateType Can_ControllerMode [MAX_NUM_OF_CAN_CONTROLLERS]                            ;
/*TODO:rename the below var CanControllerInterruptStatus*/
LOCAL uint8_t InterruptStatus [MAX_NUM_OF_CAN_CONTROLLERS]                                               ;
/*Rename this var to CANControllerIntDisableCount*/
LOCAL VAR(uint8_t,AUTOMATIC) IntDisableCount[MAX_NUM_OF_CAN_CONTROLLERS]                                 ;
/*[SWS_Can_00103] After power-up/reset, the Can module shall be in the state
CAN_UNINIT.*/
LOCAL VAR(Can_StateType,AUTOMATIC) CanUnitState = CAN_UNINIT                                             ;


/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************static functions Prototypes**************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/


LOCAL FUNC(void,CAN_CODE) Can_ConfigureHardwareObject(void);







/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_Init*********************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/




FUNC(void,CAN_CODE) Can_Init(P2VAR(Can_ConfigType,CAN_CODE,AUTOMATIC)  Config)
                            /*TODO:Initalize all variables*/                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        {
    VAR(uint8_t,AUTOMATIC)              CanDevolpmentErrorType          ;
    /*TODO:Rename this variable to BaudrateId               */
    VAR(uint16_t,AUTOMATIC)             Baudrate                        ;
    VAR(uint32_t,AUTOMATIC)             CanControllerBaseAddress        ;
    VAR(uint8_t,AUTOMATIC)              CanControllerIndex              ;
    VAR(tCANBitClkParms,AUTOMATIC)      Bit_Time_Parameters0            ; /* structure containing bit time parameters                                                      */


    if(CanUnitState == CAN_UNINIT)
        /*[SWS_Can_00246]  The function Can_Init shall change the module state to CAN_READY, after initialising all controllers inside the HW Unit.*/
    {
        /*TODO:Remove thos if cond nd add comment that indicate that we statisfy the above sws */
        if  (( Can_ControllerMode[CAN_CONTROLLER_ZERO] == CAN_CS_UNINIT ) && ( Can_ControllerMode[CAN_CONTROLLER_ONE] == CAN_CS_UNINIT ))
        {
            /*[SWS_Can_00245]  The function Can_Init shall initialize all CAN controllers according to their configuration                                             */
            for (CanControllerIndex = CAN_CONTROLLER_ZERO ; CanControllerIndex < MAX_NUM_OF_CAN_CONTROLLERS ; CanControllerIndex++ )
            {
                Baudrate                 = CanController[CanControllerIndex].CanControllerDefaultBaudrate    ;
                CanControllerBaseAddress = CanController[CanControllerIndex].CanControllerBaseAddress;
                /* Initialize Can controller Registers     (TivaWare)                                                                                                      */
                CANInit( CanControllerBaseAddress )                                                              ;
                /* store the value of Baudrate refrence to simplify the access in the below lines                                           */
                /*Fill Bit_Time_Parameters0 with time parameter from the conifguration parameters                                                                           */
                Bit_Time_Parameters0.ui32SyncPropPhase1Seg= ( (uint32_t)(CanControllerBaudrateConfig[ Baudrate ].CanControllerSeg1)   ) +
                        ( (uint32_t)(CanControllerBaudrateConfig[ Baudrate ].CanControllerPropSeg)) ;

                Bit_Time_Parameters0.ui32Phase2Seg        =   (uint32_t)CanControllerBaudrateConfig[ Baudrate ].CanControllerSeg2                         ;
                Bit_Time_Parameters0.ui32SJW              =   (uint32_t)CanControllerBaudrateConfig[ Baudrate ].CanControllerSyncJumpWidth                ;
                /*  TODO: add MCU function to get the clock speed (NOTE THAT: you should update this part if you have MCU module)   */
                Bit_Time_Parameters0.ui32QuantumPrescaler =   Mcu_GetSysCloock()/((Bit_Time_Parameters0.ui32SyncPropPhase1Seg +
                        Bit_Time_Parameters0.ui32Phase2Seg+1U) * CanControllerBaudrateConfig[ Baudrate ].CanControllerBaudRate*1000U);
                /* Configures the CAN controller bit timing      (TivaWare)                                                                                  */
                CANBitTimingSet(CanControllerBaseAddress,&Bit_Time_Parameters0)        ;
                /* One-time writable registers that require initialisation directly after reset shall be initialised by the startup code                                       */
                /*This function registers the interrupt handler in the interrupt vector table, and enables CAN interrupts on the interrupt
                controller; specific CAN interrupt sources must be enabled using CANIntEnable().                                                              */
                /*TODO:we will remove with integration with os team
                 * We need to check if the mode POLLING or INTERRUPT */
                if ( CanControllerIndex == CAN_CONTROLLER_ZERO )
                {
                    CANIntRegister(CanControllerBaseAddress,(&Can0_InterruptHandler))            ;
                    /*            initialize the CanInterruptId parameter in the Can_controller struct that shall interrupt masks that uses in Can_EnableControllerInterrupts       */
                    CanController[CAN_CONTROLLER_ZERO].CanInterruptId = (uint32_t)INT_CAN0    ;
                }
                else
                {
                    CANIntRegister(CanControllerBaseAddress,(&Can1_InterruptHandler))            ;
                    /*            initialize the CanInterruptId parameter in the Can_controller struct that shall interrupt masks that uses in Can_EnableControllerInterrupts       */
                    CanController[CAN_CONTROLLER_ONE ].CanInterruptId = (uint32_t)INT_CAN1    ;
                }
                /* only function can_Init can change controller state from uninit to stopped                                                                                     */
                /*[SWS_Can_00259]  The function Can_Init shall set all CAN controllers in the state STOPPED.  */
                /*responsible for incrementing disable_interrupt so that can enable_interrupt can execute*/
                /* after you have configured some all the can controllers in your module set this general flag                                                                   */
                Can_ControllerMode [CanControllerIndex]= CAN_CS_STOPPED                          ;


            }/* End Of the for loop */
            CanUnitState = CAN_READY                                                                                                           ;
            /* [SWS_Can_00250] The function Can_Init shall initialize:  LOCAL variables, including flags,Common setting for the complete CAN HW unitCAN                    */
            /* controller specific settings for each CAN controller                                                                                                         */
            /* [SWS_Can_00246] The function Can_Init shall change the module state to CAN_READY, after initialising all controllers inside the HW Unit.                     */
            /* Configure hardware Objects                                                                                                                           */
            Can_ConfigureHardwareObject()                                   ;
        }
        else
        {
            /*[SWS_Can_00408] If development error detection for the Can module is enabled: The function Can_Init shall raise the error CAN_E_TRANSITION*/
            /*if the CAN controllers are not in state UNINIT*/
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
            CanDevolpmentErrorType = CAN_E_TRANSITION;
            Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID_0,CAN_INIT_SID,CAN_E_TRANSITION);
#endif
        }
    }
    else
    {
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CAN_E_TRANSITION;
        Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID_0,CAN_INIT_SID,CAN_E_TRANSITION);
#endif
        /* [SWS_Can_00174] If development error detection for the Can module is enabled The function Can_Init shall raise                                                */
        /* the error CAN_E_TRANSITION if the driver is not in state CAN_UNINIT.                                                                                          */
    }

}
/* Can_ConfigureHardwareObject isn't included in SWS
 * It shall be called from Can_Init
 * to initialize the hardware object                 */



/*TODO:Comment any line of current mailbox to simplify the understanding of the code */
LOCAL FUNC(void,CAN_CODE) Can_ConfigureHardwareObject(void)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(uint8_t,AUTOMATIC) CanControllerId                                       ;
    VAR(uint8_t,AUTOMATIC) canHardwareObjectIndex                                ;
    VAR(tCANMsgObject,AUTOMATIC) CANMessage                                      ;
    VAR(uint8_t,AUTOMATIC) HO_Index                                              ;
    VAR(uint8_t,AUTOMATIC) HO_Ref[MAX_NUM_OF_CAN_CONTROLLERS] = {0,0}            ;
    VAR(uint8_t,AUTOMATIC) NumberMailBoxes_PerHO                                 ;
    VAR(uint8_t,AUTOMATIC) MailBoxIndex = 0                                      ;
    VAR(uint8_t,AUTOMATIC) CurrentMailBox[MAX_NUM_OF_CAN_CONTROLLERS] = {0,0}    ;


    /* Loop for all max hardware object in the System */
    for ( HO_Index = START_INDEX ; HO_Index < (uint8_t)MAX_NUM_OF_HO ; HO_Index++ )
    {
        CanControllerId             = CanHardwareObject[HO_Index].CanControllerRef  ;
        NumberMailBoxes_PerHO       = CanHardwareObject[HO_Index].CanHwObjectCount  ;
        CanHardwareObject[HO_Index].CanMailBoxStart = CurrentMailBox[CanControllerId] ;
        for( canHardwareObjectIndex = START_INDEX ; canHardwareObjectIndex < NumberMailBoxes_PerHO ; canHardwareObjectIndex++ )
        {
            /*TODO:Add comment for HO_Ref */
            Can_MailBoxLookUpTables[CanControllerId][MailBoxIndex++].HwObject = HO_Ref[CanControllerId];
            if ( CanHardwareObject[HO_Index].CanObjectType == (CanObjectType_t)RECEIVE )
            {
                /*psMsgObject->ui32MsgIDMask bits that are used for masking during comparison.                   (TivaWare) */
                /*Set psMsgObject->ui32MsgID to the full message ID, or a partial mask to use partial ID matching.(TivaWare)*/
                /*TODO: add struct for the filter and make array for the cfg filter then with tool integration */
                CANMessage.ui32MsgID     =  CanHardwareObject[HO_Index].CanHwFilterCode ;
                CANMessage.ui32MsgIDMask =  CanHardwareObject[HO_Index].CanHwFilterMask ;

                /*Set psMsgObject->ui32MsgLen to the number of bytes in the expected data frame.         (TivaWare)          */
                if ( CanHardwareObject[HO_Index].CanIdType ==  EXTENDED )
                {
                    /* \e psMsgObject->ui32Flags as follows: Set  MSG_OBJ_RX_INT_ENABLE flag to be interrupted when the data frame is received.
                                                             Set  MSG_OBJ_USE_ID_FILTER flag to enable identifier-based filtering.(TivaWare)*/
                    CANMessage.ui32Flags = (uint32_t)MSG_OBJ_RX_INT_ENABLE| (uint32_t)MSG_OBJ_USE_ID_FILTER|(uint32_t)MSG_OBJ_EXTENDED_ID                     ;
                }
                else
                {
                    CANMessage.ui32Flags = (uint32_t)MSG_OBJ_RX_INT_ENABLE|(uint32_t)MSG_OBJ_USE_ID_FILTER                                          ;
                }
                /*Configures a message object in the CAN controller.(TivaWare)   Number from 1 to 32                                                       */
                CANMessageSet( CanController[CanControllerId].CanControllerBaseAddress, MailBoxIndex ,&CANMessage, MSG_OBJ_TYPE_RX )         ;
            }
            else
            {
                /* MISRA */
            }
            CurrentMailBox[CanControllerId]++ ;
        }
        HO_Ref[CanControllerId]++;

    }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }

/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_DeInit*******************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/


FUNC(void,CAN_CODE) Can_DeInit(void)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(uint8_t,AUTOMATIC)         CanDevolpmentErrorType          ;
    VAR(uint8_t,AUTOMATIC)         Controller                      ;

    if(CanUnitState == CAN_READY ) /*[SWS_Can_91011]  development error detection for the Can module is enabled:
                                     The function Can_DeInit shall raise the error CAN_E_TRANSITION if the
                                       driver is not in state CAN_READY.(SRS_BSW_00369) */
    {
        /*[SWS_Can_91012] If development error detection for the Can module is enabled: The function Can_DeInit shall
        raise the error CAN_E_TRANSITION if any of the CAN controllers is in state STARTED.*/
        /*  Caveat: Caller of the Can_DeInit function has to be sure no CAN controller is in the state STARTED*/

        if  ( (Can_ControllerMode [CAN_CONTROLLER_ZERO]!= CAN_CS_STARTED ) && ( Can_ControllerMode[CAN_CONTROLLER_ONE]!= CAN_CS_STARTED) )
        {
            CanUnitState = CAN_UNINIT ;  /*   [SWS_Can_ 91009] The function Can_DeInit shall change the module state to
                               CAN_UNINIT before de-initialising all controllers inside the HW unit.*/
            for (Controller = CAN_CONTROLLER_ZERO ; Controller < MAX_NUM_OF_CAN_CONTROLLERS ; Controller++)
            {
                CANDisable( CanController[Controller].CanControllerBaseAddress );
                Can_ControllerMode [Controller] = CAN_CS_UNINIT                 ;
            }
        }
        else{
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
            CanDevolpmentErrorType = CAN_E_TRANSITION;
            Det_ReportError(CAN_MODULE_ID,Controller,CAN_DEINIT_SID,CAN_E_TRANSITION);
#endif
        }
    }
    else
    {
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CAN_E_TRANSITION;
        Det_ReportError(CAN_MODULE_ID,Controller,CAN_DEINIT_SID,CAN_E_TRANSITION);
#endif
    }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }





/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_SetBaudrate**************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/


#if CAN_SET_BAUDRATE_API == TRUE
FUNC(Std_ReturnType,CAN_CODE) Can_SetBaudrate(VAR(uint8_t,AUTOMATIC) Controller,VAR(uint16_t,AUTOMATIC) BaudRateConfigID )
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR( uint8_t , AUTOMATIC )         CanDevolpmentErrorType ;
    VAR( uint8_t , AUTOMATIC )         ErrorStatus = E_OK     ;
    VAR( tCANBitClkParms , AUTOMATIC) Bit_Time_Parameters     ;

    if( CanUnitState != CAN_READY)
    {
        /*  [SWS_CAN_00492] If development error detection for the Can module is enabled:
       Specification of CAN Driver
        The function Can_SetBaudrate shall raise the error CAN_E_UNINIT and return
       E_NOT_OK if the driver is not yet init_ialized.*/
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CAN_E_UNINIT;
        Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID_0,CAN_SET_BAUDRATE_SID,CAN_E_UNINIT);
#endif
        ErrorStatus = E_NOT_OK;
    }
    else
    {
        /* MISRA */
    }
    if( Controller >=  MAX_NUM_OF_CAN_CONTROLLERS )
    {
        /*[SWS_CAN_00494] If development error detection for the Can module is enabled
                 the function Can_SetBaudrate shall raise the error CAN_E_PARAM_CONTROLLER and
                 return E_NOT_OK if the parameter Controller is out of range.*/
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CAN_E_PARAM_CONTROLLER;
        Det_ReportError(CAN_MODULE_ID,Controller,CAN_SET_BAUDRATE_SID,CAN_E_PARAM_CONTROLLER);
#endif
        ErrorStatus = E_NOT_OK;
    }
    else
    {
        /* MISRA */
    }
    if(BaudRateConfigID >= MAX_NUM_OF_BAUDRATES )
    {
        /* [SWS_CAN_00493] If development error detection for the Can module is enabled:
                        The function Can_SetBaudrate shall raise the error CAN_E_PARAM_BAUDRATE
                               and return E_NOT_OK if the parameter BaudRateConfigID has an invalid value.*/
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType=CAN_E_PARAM_BAUDRATE;
        Det_ReportError(CAN_MODULE_ID,Controller,CAN_SET_BAUDRATE_SID,CAN_E_PARAM_BAUDRATE);
#endif
        ErrorStatus = E_NOT_OK;
    }
    else
    {
        /* MISRA */
    }
    if ( ErrorStatus == E_OK)

    {
        Bit_Time_Parameters.ui32SyncPropPhase1Seg = CanControllerBaudrateConfig[BaudRateConfigID ].CanControllerSeg1+
                CanControllerBaudrateConfig[BaudRateConfigID ].
                CanControllerPropSeg;
        Bit_Time_Parameters.ui32Phase2Seg=CanControllerBaudrateConfig[BaudRateConfigID ].CanControllerSeg2;
        Bit_Time_Parameters.ui32SJW=CanControllerBaudrateConfig[BaudRateConfigID ].CanControllerSyncJumpWidth;
        Bit_Time_Parameters.ui32QuantumPrescaler=SysCtlClockGet()/((Bit_Time_Parameters.ui32SyncPropPhase1Seg
                +Bit_Time_Parameters.ui32Phase2Seg+1U)*
                CanControllerBaudrateConfig[BaudRateConfigID ].CanControllerBaudRate*1000U);


        CANBitTimingSet(CanController[Controller].CanControllerBaseAddress,&Bit_Time_Parameters);
        if( Can_ControllerMode [Controller]==CAN_CS_STOPPED||Can_ControllerMode [Controller]==CAN_CS_SLEEP)
        {
            CANDisable(CanController[Controller].CanControllerBaseAddress);
        }else
        {
            /*MISRA*/
        }
        ErrorStatus = E_OK;
    }
    else
    {
        ErrorStatus= E_NOT_OK;
    }
    return ErrorStatus ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }
#endif

/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_SetControllerMode********************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/

/* SWS_Can_00230: This function performs software triggered state transitions of the CAN controller State machine
 *                 Which means that this function changes the state of the controller between started, stopped and sleep
 */


FUNC(Std_ReturnType,CAN_CODE) Can_SetControllerMode( VAR(uint8_t,AUTOMATIC) Controller, VAR(Can_StateTransitionType,AUTOMATIC) Transition )
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(uint8_t,AUTOMATIC)        HwObject               ;
    VAR(Std_ReturnType,AUTOMATIC) toBeReturned = E_OK         ;
    VAR(uint8_t,AUTOMATIC)        CanDevolpmentErrorType      ;

    /*  Check for the initialization of the Driver   */
    if(CanUnitState != CAN_UNINIT)
    {
        if(Controller < MAX_NUM_OF_CAN_CONTROLLERS)
        {
            /*   [SWS_Can_00409]  When the function Can_SetControllerMode (CAN_CS_STARTED) is entered and the
                CAN controller is not in state STOPPED it shall detect a invalid state transition*/
            if(((Transition == ( Can_StateTransitionType ) CAN_CS_STARTED) && ( Can_ControllerMode [Controller] == ( Can_StateTransitionType ) CAN_CS_STOPPED))|| ((Transition == ( Can_StateTransitionType )CAN_CS_SLEEP) && (Can_ControllerMode [Controller] == ( Can_StateTransitionType )CAN_CS_STOPPED)))
            {
                /*  this flag is set here to indicate to Can_Mainfunction_Mode that a change has been made
                 *  it is also cleared at the Can_Mainfunction_Mode
                 */
                state_transition_flag[Controller] = MODE_CHANGED       ;

                switch(Transition)
                {

                case CAN_CS_STARTED :
                    /*[SWS_Can_00196]The function Can_SetControllerMode shall enable interrupts that are needed in the new state.*/

                    /* [SWS_Can_00261] The function Can_SetControllerMode(CAN_CS_STARTED) shall set the hardware registers in a way
                              that makes the CAN controller participating on the network.*/

                    /*[SWS_Can_00204] The Can module shall track all individual enabling and disabling of interrupts in other functions (i.e. Can_SetControllerMode)
                            , so that the correct interrupt enable state can be restored.*/

                    /* [SWS_Can_00425] Enabling of CAN interrupts shall not be executed, when CAN interrupts have been disabled by function Can_DisableControllerInterrupts.*/
                    if( OUT_CRITICAL_SECTION()  && InterruptStatus[Controller] != INTERRUPT_ENABLE )
                    {
                        CANIntEnable(CanController[Controller].CanControllerBaseAddress, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                        IntEnable(CanController[Controller].CanInterruptId)                                                              ;
                        CANEnable(CanController[Controller].CanControllerBaseAddress)                                                    ;
                        InterruptStatus[Controller] = INTERRUPT_ENABLE                                                                   ;
                    }
                    /* if the interrupt has been enabled before hand */
                    else
                    {
                        CANEnable(CanController[Controller].CanControllerBaseAddress);
                    }

                    /*  Changing the global controller mode  */
                    Can_ControllerMode[Controller] = CAN_CS_STARTED;
                    break;

                case CAN_CS_STOPPED :

                    /* if the interrupt has been disabled before hand */
                    if( IN_CRITICAL_SECTION() )
                    {
                        CANDisable(CanController[Controller].CanControllerBaseAddress);
                    }
                    /*  if this is the first time to disabled the interrupt  */
                    else
                    {
                        InterruptStatus[Controller] = INTERRUPT_DISABLE                                                                   ;
                        CANDisable (CanController[Controller].CanControllerBaseAddress)                                                   ;
                        CANIntDisable(CanController[Controller].CanControllerBaseAddress, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                    }

                    /* [SWS_Can_00263] The function Can_SetControllerMode(CAN_CS_STOPPED) shall set theThis message object is not waiting for transmission.
                            bits inside the CAN hardware such that the CAN controller stops participating on the network.*/
                    for( HwObject = START_INDEX ; HwObject < MAX_NUM_OF_HO ; HwObject++ )
                    {
                        if( CanHardwareObject[HwObject].CanObjectType == (CanObjectType_t)TRANSMIT)
                        {
                            if(Controller == CAN_CONTROLLER_ZERO)   /*  if this is Can0 then use the registers from Can0 (Note That: if you are accessing the registers at the whole code, please use struct and remove this if)  */
                            {
                                /* Selects one of the 32 message objects in the message RAM for data transfer. The message objects are numbered from 1 to 32. */
                                /* This message object is not waiting for transmission.*/
                                CAN0_IF1CRQ_R   = ((uint32_t) ( Can_MailBoxLookUpTables[Controller][HwObject].HwObject ) );

                                CAN0_IF1MCTL_R &= ( ( uint32_t )( ~MESSAGE_WAITING_MASK ) )                                      ;
                                CAN0_IF1CMSK_R &= ( ( uint32_t )( ~IF1CMSK_MASK ) )                                              ;
                            }
                            else if( Controller == CAN_CONTROLLER_ONE )
                            {
                                CAN1_IF1CRQ_R   = ( (uint32_t )( Can_MailBoxLookUpTables[Controller][(HwObject)].HwObject ) );
                                CAN1_IF1MCTL_R &= ( (uint32_t )( ~MESSAGE_WAITING_MASK))                                          ;
                                CAN1_IF1CMSK_R &= ( (uint32_t )( ~IF1CMSK_MASK ) )                                                ;
                            }
                        }
                        else
                        {/*MISRA*/}
                    }
                    Can_ControllerMode[Controller] = CAN_CS_STOPPED ;
                    break;



                    /*[SWS_Can_00258] When the CAN hardware does not support sleep mode and is triggered to transition into SLEEP state,
                            the Can module shall emulate a logical SLEEP state from which it returns only, when it is triggered by software to transition into STOPPED state.*/
                    /*[SWS_Can_00404] The CAN hardware shall remain in state STOPPED, while the logical SLEEP state is active.*/
                    /*[SWS_Can_00290] If the CAN HW does not support a sleep mode,
                            the function Can_SetControllerMode(CAN_CS_SLEEP) shall set the CAN controller to the logical sleep mode.*/
                    /* [SWS_Can_00197] The function Can_SetControllerMode shall disable interrupts that are not allowed in the new state. */
                case CAN_CS_SLEEP :
                    /* if the interrupt has been disabled before hand */
                    if(IN_CRITICAL_SECTION())
                    {

                        CANDisable(CanController[Controller].CanControllerBaseAddress);

                    }
                    /*  if this is the first time to disabled the interrupt  */
                    else
                    {
                        InterruptStatus[Controller] = INTERRUPT_DISABLE                                                                     ;
                        CANDisable ( CanController[Controller].CanControllerBaseAddress )                                                   ;
                        CANIntDisable( CanController[Controller].CanControllerBaseAddress, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS );
                    }

                    /* [SWS_Can_00263] The function Can_SetControllerMode(CAN_CS_STOPPED) shall set theThis message object is not waiting for transmission.
                            bits inside the CAN hardware such that the CAN controller stops participating on the network.*/

                    for( HwObject = START_INDEX ; HwObject < MAX_NUM_OF_HO ; HwObject++)
                    {
                        if( CanHardwareObject[HwObject].CanObjectType==TRANSMIT)
                        {
                            if( Controller == CAN_CONTROLLER_ZERO )   /*  if this is Can0 then use the registers from Can0 (Note That: if you are accessing the registers at the whole code, please use struct and remove this if)  */
                            {
                                /* Selects one of the 32 message objects in the message RAM for data transfer. The message objects are numbered from 1 to 32. */
                                CAN0_IF1CRQ_R  = ((uint32_t)(Can_MailBoxLookUpTables[Controller][HwObject].HwObject));
                                CAN0_IF1MCTL_R &= ((uint32_t)(~MESSAGE_WAITING_MASK)); /* This message object is not waiting for transmission.*/
                                CAN0_IF1CMSK_R &= ((uint32_t)(~IF1CMSK_MASK));
                            }
                            else if( Controller == CAN_CONTROLLER_ONE )
                            {
                                CAN1_IF1CRQ_R   = ( (uint32_t)( Can_MailBoxLookUpTables[Controller][(HwObject)].HwObject ) ) ;
                                CAN1_IF1MCTL_R &= ( (uint32_t)( ~MESSAGE_WAITING_MASK))                                      ;
                                CAN1_IF1CMSK_R &= ( (uint32_t)( ~IF1CMSK_MASK))                                              ;
                            }else
                            {}
                        }
                        else
                        {/*MISRA*/}
                    }
                    Can_ControllerMode[Controller] = CAN_CS_SLEEP;
                    break;
                default:
                    break;
                }
            }

            /*   if the input parameter Transition is not valid, then change the error type and return E_NOT_OK indication that the operation did not take place   */
            else
            {
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
                CanDevolpmentErrorType=CAN_E_TRANSITION;
                Det_ReportError(CAN_MODULE_ID,Controller,CAN_SET_CONTROLLER_MODE_SID,CanDevolpmentErrorType);
#endif

                toBeReturned = E_NOT_OK;

            }
        }
        /*   if the input parameter Controller is not valid, then change the error type and return E_NOT_OK indication that the operation did not take place   */
        else
        {
            toBeReturned = E_NOT_OK                          ;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
            CanDevolpmentErrorType = CAN_E_PARAM_CONTROLLER  ;
            Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID_0,CAN_SET_CONTROLLER_MODE_SID,CanDevolpmentErrorType);
#endif

        }
    }


    else
    {

#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CAN_E_PARAM_CONTROLLER  ;
        Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID_0,CAN_INIT_SID,CanDevolpmentErrorType);
#endif
        toBeReturned = E_NOT_OK;
    }

    /**********************************************************************************************************************/
    /*[SWS_Can_00262] The function Can_SetControllerMode(CAN_CS_STARTED) shall wait for
 limited time until the CAN controller is fully operational. Compare to SWS_Can_00398.*/

    /*[SWS_Can_00264] The function Can_SetControllerMode(CAN_CS_STOPPED) shall wait for
a limited time until the CAN controller is really switched off. Compare to SWS_Can_00398.*/

    return toBeReturned;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_DisableControllerInterrupts**********************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/





FUNC(void,CAN_CODE)  Can_DisableControllerInterrupts(VAR(uint8_t,AUTOMATIC) Controller)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(uint8_t, AUTOMATIC) CanDevolpmentErrorType ;
    if(CanUnitState!=CAN_UNINIT)
    {
        if(Controller < MAX_NUM_OF_CAN_CONTROLLERS)
        {
            /*[SWS_Can_00202] When Can_DisableControllerInterrupts has been called several times,
         Can_EnableControllerInterrupts must be called as many times before the interrupts are re-enabled.*/
            /*  in other words this counter increases everytime you call the disable function and to re-enable you need to
                call this function with the same amount.  */

            if( OUT_CRITICAL_SECTION() )
            {
                CANIntDisable(CanController[Controller].CanControllerBaseAddress, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                IntDisable(CanController[Controller].CanInterruptId);
                InterruptStatus[Controller] = INTERRUPT_DISABLE ;

            }
            else
            {

            }
            ENTER_CRITICAL_SECTION(Controller);
        }
        else
        {

#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
            CanDevolpmentErrorType = CAN_E_PARAM_CONTROLLER  ;
            Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID_0,CAN_DISABLE_CONTROLLER_INTERRUPTS_SID,CanDevolpmentErrorType);
#endif
        }
    }
    else
    {
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CAN_E_UNINIT;
        Det_ReportError(CAN_MODULE_ID,Controller,CAN_DISABLE_CONTROLLER_INTERRUPTS_SID,CanDevolpmentErrorType);
#endif
    }

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_EnableControllerInterrupts**********************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/




/*TODO:this function shall be put with #if  */
FUNC(void,CAN_CODE)  Can_EnableControllerInterrupts(VAR(uint8_t,AUTOMATIC) Controller)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(uint8_t,AUTOMATIC) CanDevolpmentErrorType;
    if(CanUnitState!=CAN_UNINIT)
    {
        if(Controller < MAX_NUM_OF_CAN_CONTROLLERS)
        {

            if( Can_ControllerMode[Controller] == CAN_CS_SLEEP || Can_ControllerMode[Controller] == CAN_CS_STOPPED)
            {

                /* [SWS_Can_00208] The function Can_EnableControllerInterrupts shall perform no action when
                      Can_DisableControllerInterrupts has not been called before.*/
                /*TODO: IS_CAN_CONTROLLER_INTERRUPT_ENABLE Rename this macro  == TRUE or FLASE */
                if(IN_CRITICAL_SECTION())

                {
                    /*[SWS_Can_00202]   When Can_DisableControllerInterrupts has been called several times,
                                    Can_EnableControllerInterrupts must be called as many times before the interrupts are re-enabled.*/
                    /*  in other words this counter increases everytime you call the disable function and to re-enable you need to
                        call this function with the same amount.  */
                    /*  this is a macro like function, you can find it in Can_interrupt.c   */
                    EXIT_CRITICAL_SECTION(Controller);
                    if( OUT_CRITICAL_SECTION() && InterruptStatus[Controller] != INTERRUPT_ENABLE )
                    {
                        /* set interrupt enable, status interrupt enable, error interrupt enable to disable interrupts."*/
                        CANIntEnable(CanController[Controller].CanControllerBaseAddress, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                        IntEnable(CanController[Controller].CanInterruptId);
                        InterruptStatus[Controller] = INTERRUPT_ENABLE ;
                    }
                    else
                    {

                    }
                }
                else
                {
                }
            }
            else
            {

            }
        }
        else
        {

#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
            CanDevolpmentErrorType = CAN_E_PARAM_CONTROLLER;
            Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID_0,CAN_ENABLE_CONTROLLER_INTERRUPTS_SID,CanDevolpmentErrorType);
#endif
        }
    }
    else {
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType=CAN_E_UNINIT;
        Det_ReportError(CAN_MODULE_ID,Controller,CAN_ENABLE_CONTROLLER_INTERRUPTS_SID,CanDevolpmentErrorType);
#endif
    }


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }


/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_GetControllerErrorState**************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/



/* TOBEASKED : In sws the implementation must be enum  ??????????*/
#define CAN_ERRORSTATE_BUSOFF  BOFF
#define CAN_ERRORSTATE_ACTIVE  EWARN
#define CAN_ERRORSTATE_PASSIVE EPASS

FUNC(Std_ReturnType,CAN_CODE) Can_GetControllerErrorState( VAR(uint8_t,AUTOMATIC) ControllerId,P2VAR(Can_ErrorStateType,CAN_CODE,AUTOMATIC)  ErrorStatePtr )
                                                                                                                                                                                                                                                        {

    VAR(Std_ReturnType,AUTOMATIC) RetuenValue =  E_OK ;
    VAR(uint8_t,AUTOMATIC) CanDevolpmentErrorType = NO_ERROR;

    if( CanUnitState==CAN_UNINIT )
    {
        /*[SWS_Can_91005] If development error detection for the Can module is enabled: if the module is not yet initialized,
    the function Can_GetControllerErrorState shall raise development error CAN_E_UNINIT and return E_NOT_OK.*/
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CAN_E_UNINIT;
        Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID_0,CAN_GET_CONTROLLER_ERROR_STATUS_SID,CanDevolpmentErrorType);
#endif
        RetuenValue = E_NOT_OK;
    }
    if( ControllerId >= MAX_NUM_OF_CAN_CONTROLLERS)
    {
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CAN_E_PARAM_CONTROLLER;
        Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID_0,CAN_GET_CONTROLLER_ERROR_STATUS_SID,CanDevolpmentErrorType);
#endif
        RetuenValue = E_NOT_OK;
    }
    if( ErrorStatePtr == NULL_PTR )
    {
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CAN_E_PARAM_POINTER ;
        Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID_0,CAN_GET_CONTROLLER_ERROR_STATUS_SID,CanDevolpmentErrorType);
#endif
        RetuenValue = E_NOT_OK                       ;
    }

    if ( RetuenValue == E_NOT_OK )
    {
        /* Reads one of the controller status registers. (Tivaware)*/
        /* - \b CAN_STATUS_BUS_OFF - controller is in bus-off condition
    //TOBEASKED : there are three types of error only that are defines
    //! - \b CAN_STATUS_EWARN - an error counter has reached a limit of at least 96
    //! - \b CAN_STATUS_EPASS - CAN controller is in the error passive state
    //! - \b CAN_STATUS_RXOK - a message was received successfully (independent of
    //!   any message filtering).
    //! - \b CAN_STATUS_TXOK - a message was successfully transmitted
    //! - \b CAN_STATUS_LEC_MSK - mask of last error code bits (3 bits)
    //! - \b CAN_STATUS_LEC_NONE - no error
    //! - \b CAN_STATUS_LEC_STUFF - stuffing error detected
    //! - \b CAN_STATUS_LEC_FORM - a format error occurred in the fixed format part
    //!   of a message
    //! - \b CAN_STATUS_LEC_ACK - a transmitted message was not acknowledged
    //! - \b CAN_STATUS_LEC_BIT1 - dominant level detected when trying to send in
    //!   recessive mode
    //! - \b CAN_STATUS_LEC_BIT0 - recessive level detected when trying to send in
    //!   dominant mode
    //! - \b CAN_STATUS_LEC_CRC - CRC error in received message
    //!*/
        *ErrorStatePtr  = CANStatusGet(CanController[ControllerId].CanControllerBaseAddress, CAN_STS_CONTROL) ;
        *ErrorStatePtr &= 0xE0U                                                                                 ;
        RetuenValue = E_OK;
    }
    else
    {
        /*MISRA*/
    }
    return RetuenValue ;
                                                                                                                                                                                                                                                        }

/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_GetControllerMode********************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/



/*[SWS_Can_91015] The service Can_GetControllerMode shall return the mode of the requested CAN controller.*/
FUNC (Std_ReturnType,CAN_CODE) Can_GetControllerMode( VAR(uint8_t,AUTOMATIC) Controller, P2VAR(Can_ControllerStateType,CAN_CODE,AUTOMATIC)  ControllerModePtr)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(Std_ReturnType,AUTOMATIC) toBeReturned = E_OK;
    VAR(uint8_t,AUTOMATIC) CanDevolpmentErrorType;
    /*  validating inputs  */
    if(Controller < MAX_NUM_OF_CAN_CONTROLLERS)
    {
        if(ControllerModePtr!=NULL_PTR)
        {
            /*  check that the can has been initialized before  */
            if(CanUnitState == CAN_READY )
            {
                *ControllerModePtr=Can_ControllerMode [Controller];
            }
            else
            {
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
                CanDevolpmentErrorType = CAN_E_UNINIT;
                Det_ReportError(CAN_MODULE_ID,Controller,CAN_GET_CONTROLLER_MODE_SID ,CanDevolpmentErrorType);
#endif
                toBeReturned = E_NOT_OK;
            }
        }
        else
        {
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
            CanDevolpmentErrorType = CAN_E_PARAM_POINTER;
            Det_ReportError(CAN_MODULE_ID,Controller,CAN_GET_CONTROLLER_MODE_SID ,CanDevolpmentErrorType);
#endif
            toBeReturned = E_NOT_OK;
        }
    }
    else
    {
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType=CAN_E_PARAM_CONTROLLER;
        Det_ReportError(CAN_MODULE_ID,CAN_INSTANCE_ID_0,CAN_GET_CONTROLLER_MODE_SID ,CanDevolpmentErrorType);
#endif
        toBeReturned = E_NOT_OK;
    }

    return toBeReturned;

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }

/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_Write********************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/


/*[SWS_Can_00275] The function Can_Write shall be non-blocking.*/
FUNC(Std_ReturnType,CAN_CODE) Can_Write(VAR(Can_HwHandleType,AUTOMATIC) HTH ,CONSTP2VAR(Can_PduType,CAN_CODE,AUTOMATIC) PduInfo)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(uint8_t,AUTOMATIC)         CanDevolpmentErrorType = NO_ERROR     ;
    VAR(tCANMsgObject,AUTOMATIC)    sCANMessage                          ;
    VAR(uint32_t,AUTOMATIC)        REGISTER_1                            ;
    VAR(uint32_t,AUTOMATIC)        REGISTER_2                            ;
    VAR(Std_ReturnType ,AUTOMATIC)      ErrorStatus  = E_OK              ;
    VAR(uint8_t,AUTOMATIC)         MailBoxIndex                          ;
    VAR(uint8_t,AUTOMATIC)         CanControllerId                       ;

    /* [SWS_Can_00100] Several TX hardware objects with unique HTHs may be configured. The
            CanIf module provides the HTH as parameter of the TX request.*/

    /*[SWS_Can_00276] The function Can_Write shall store the swPduHandle that is given
            inside the parameter PduInfo until the Can module calls the CanIf_TxConfirmation
            for this request where the swPduHandle is given as parameter.*/
    /*canTxPDUid_confirmation[pdu_id]=un_sent;*/

    if( CanUnitState == CAN_UNINIT )
    {
        /*[SWS_Can_00216] If development error detection for the Can module is enabled: The function
                Can_Write shall raise the error CAN_E_UNINIT and shall return E_NOT_OK if the driver is not yet initia_lized.*/

#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CAN_E_UNINIT ;
        Det_ReportError(CAN_MODULE_ID , CAN_INSTANCE_ID_0,CAN_WRITE_SID ,CanDevolpmentErrorType);
#endif
        ErrorStatus            = E_NOT_OK     ;
    }
    if ( HTH >= MAX_NUM_OF_HO  &&  CanHardwareObject[HTH].CanObjectType == RECEIVE )
    {
        /*[SWS_Can_00217] If development error detection for the Can module is enabled: The function Can_Write
                shall raise the errorCAN_E_PARAM_HANDLE and shall return E_NOT_OK if the parameter Hth is not a
                configured Hardware transmit Handle.*/
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CAN_E_PARAM_HANDLE;
        Det_ReportError(CAN_MODULE_ID , CAN_INSTANCE_ID_0 ,CAN_WRITE_SID ,CanDevolpmentErrorType);
#endif
        ErrorStatus            = E_NOT_OK     ;

    }
    if( PduInfo == NULL_PTR  || PduInfo->sdu == NULL_PTR)
    {

        /*([SWS_CAN_00503] Can_Write() shall accept a NULL_PTR pointer as SDU (Can_PduType.Can_SduPtrType = NULL_PTR)
                if the trigger transmit API is enabled for this hardware object (CanTriggerTransmitEnable = TRUE).*/

        /*[SWS_CAN_00504] If the trigger transmit API is enabled for the hardware object, Can_Write() shall interpret a NULL_PTR_PTR pointer as
                SDU (Can_PduType.Can_SduPtrType = NULL_PTR_PTR) as request for using the trigger transmit interface.
                If so and the hardware object is free, Can_Write() shall call CanIf_TriggerTransmit() with the maximum size of the message buffer
                 to acquire the PDU�s data. NOT SUPPORTED */

        /*[SWS_CAN_00219] If development error detection for CanDrv is enabled: Can_Write()
                shall raise CAN_E_PARAM_POINTER and shall return E_NOT_OK if the parameter PduInfo
                is a NULL_PTR_PTR pointer.*/

        /*[SWS_CAN_00505] If development error detection for CanDrv is enabled: Can_Write()
                shall raise CAN_E_PARAM_POINTER and shall return E_NOT_OK if the trigger transmit
                API is disabled for this hardware object (CanTriggertransmitEnable = FALSE) and
                the SDU pointer inside PduInfo is a NULL_PTR_PTR pointer.*/
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CAN_E_PARAM_POINTER;
        Det_ReportError(CAN_MODULE_ID , CAN_INSTANCE_ID_0 ,CAN_WRITE_SID ,CanDevolpmentErrorType);
#endif
        ErrorStatus            = E_NOT_OK     ;
    }


    if(PduInfo->length> MAX_DATA_LENGTH)
    {
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CAN_E_PARAM_DATA_LENGTH;
        Det_ReportError(CAN_MODULE_ID , CAN_INSTANCE_ID_0 ,CAN_WRITE_SID ,CanDevolpmentErrorType);
#endif

        ErrorStatus            = E_NOT_OK     ;
        /*[SWS_Can_00218] The function Can_Write shall return E_NOT_OK and if development error detection
                          for the CAN module is enabled shall raise the error CAN_E_PARAM_DATA_LENGTH,
                          If the length is more than 8 byte*/
    }
    if (ErrorStatus == E_OK  )
    {
        {
            CanControllerId  = CanHardwareObject[HTH].CanControllerRef ;
            for( MailBoxIndex = CanHardwareObject[HTH].CanMailBoxStart ; MailBoxIndex < ( CanHardwareObject[HTH].CanHwObjectCount + CanHardwareObject[HTH].CanMailBoxStart) ; MailBoxIndex ++)
            {
                /*TODO: Shall add break if you find a  first mailbox free */
                /* Check if the corresponde mailbox is empty or not                             */
                if ( Can_MailBoxLookUpTables[ CanControllerId ][ MailBoxIndex ].Confirmation == CONFIRMED )
                {
                    /* Set confirmation to the correspond mailbox                           */
                    Can_MailBoxLookUpTables [ CanControllerId ][ MailBoxIndex ] .Confirmation = UNCONFIRMED                ;
                    /* Map the mailboc to pdu for the Rxindation in the ISR                 */
                    Can_MailBoxLookUpTables [ CanControllerId ][ MailBoxIndex ] .pduId        = PduInfo->swPduHandle       ;

                    sCANMessage.ui32MsgID     =  (uint32_t)PduInfo ->id                                       ;
                    sCANMessage.ui32MsgIDMask =  (uint32_t)0U                                                 ;
                    sCANMessage.ui32MsgLen    =  (uint32_t)PduInfo->length                                    ;
                    sCANMessage.pui8MsgData   = (uint32_t*)PduInfo ->sdu                                       ;

                    /*[SWS_CAN_00502]  If PduInfo->SduLength does not match possible DLC values CanDrv
                                                shall use the next higher valid DLC for transmission with initialisation of unused bytes
                                                to the value of the corresponding CanFdPaddingValue (see ECUC_Can_00485).*/

                    if( PduInfo->length < MIN_DATA_SIZE )
                    {
                        sCANMessage.ui32MsgLen   =   (uint32_t) MIN_DATA_SIZE                                                                      ;
                    }
                    else
                    {
                        sCANMessage.ui32MsgLen  =     (uint32_t)(PduInfo->length)                                                          ;
                    }

                    /* [SWS_Can_00059] Data mapping by CAN to memory is defined in a way that the
                                                  CAN data byte which is sent out first is array element 0, the CAN data byte which
                                                  is sent out last is array element 7 or 63 in case of CAN FD.
                                                 N.B:Our TIVA c doesn't support FD*/
                    CanControllerId = CanHardwareObject[HTH].CanControllerRef                 ;
                    if( CanHardwareObject[HTH].CanIdType == EXTENDED )
                    {
                        sCANMessage.ui32Flags =  ((uint32_t) MSG_OBJ_TX_INT_ENABLE | (uint32_t)MSG_OBJ_EXTENDED_ID )       ;
                    }
                    else
                    {
                        sCANMessage.ui32Flags = (uint32_t)MSG_OBJ_TX_INT_ENABLE                           ;
                    }
                    /*TODO:#if DEBUG == TRUE */
                    /* For test only */
                    //                    UARTprintf("MailBoxSent is %d \n",MailBoxIndex+1);

                    /*Configures a message object in the CAN controller.(TivaWare)                                                          */
                    CANMessageSet( CanController[CanControllerId].CanControllerBaseAddress, MailBoxIndex+1 ,&sCANMessage, MSG_OBJ_TYPE_TX )         ;

                }
                else
                {
                    ErrorStatus            =  CAN_BUSY    ;/*add break here when u clear multi exit points of function*/
                }
            }/*End of for Loop */
        }

        /*[SWS_Can_00011] The Can module shall directly copy the data from the upper
                          layer buffers. It is the responsibility of the upper layer to keep the buffer consistent until return
                          of function call*/
    }
    else
    {
        /* MISRA */
    }
    return ErrorStatus ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }



/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_MainFunction_Write******************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/

/*[SWS_Can_00031] The function Can_MainFunction_Write shall perform the polling of TX confirmation when CanTxProcessingis
set to POLLING or mixed. In case of mixed processing only the hardware objects for which CanHardwareObjectUsesPolling is set
 to TRUE shall be polled (SRS_BSW_00432, SRS_BSW_00373, SRS_SPAL_00157)*/
#if ( CAN_CONTROLLER_CAN_TX_PROCESSING == POLLING )
FUNC(void,CAN_CODE)  Can_MainFunction_Write(void)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(uint8_t,AUTOMATIC) CanMailBoxIndex              ;
    P2VAR(uint16_t,CAN_CODE,AUTOMATIC) pollinRegister  ;
    P2VAR(uint16_t,CAN_CODE,AUTOMATIC) PtrToReg  ;
    VAR(uint8_t,AUTOMATIC) Controller = 0    ;
    VAR(uint8_t,AUTOMATIC) ConfirmedMailBoxIndex        ;
    VAR(uint8_t,AUTOMATIC) HO_Index        ;

    /*Clear 7th bit in CANx_IF1CMSK_R */
    /* Transfer the data in the CAN message object specified by the MNUM field in the CANIFnCRQ register into the CANIFn registers.*/

    /*TODO:CLR_BIT_KAZA*/
    CAN0_IF1CMSK_R&= ((uint32_t)(~IF1CMSK_MASK));
    CAN1_IF1CMSK_R&= ((uint32_t)(~IF1CMSK_MASK));

    /* MISRA violation */
    /*casting pointer to integral type unavoidable when addressing memory mapped registers
          or other hardware specific features.*/

    /*TODO: Loop for the TX hwobjects */
    for ( Controller = CAN_CONTROLLER_ZERO ; Controller < MAX_NUM_CONTROLLER ; Controller++ )
    {
        /*TODO: Add var MAX_Configured_MB  */
        for( CanMailBoxIndex = 0U ; CanMailBoxIndex < MAX_NUM_OF_MAILBOXES ; CanMailBoxIndex++ )
        {

            HO_Index = Can_MailBoxLookUpTables[Controller][CanMailBoxIndex].HwObject;
            if( CanHardwareObject[ HO_Index ] .CanObjectType== TRANSMIT )
            {
                if( Controller == CAN_CONTROLLER_ZERO )
                {
                    PtrToReg = ( uint16_t* )&CAN0_IF1MCTL_R ;
                }
                else
                {
                    PtrToReg = ( uint16_t* ) &CAN1_IF1MCTL_R ;
                }
                if( Can_MailBoxLookUpTables[Controller][CanMailBoxIndex].Confirmation == UNCONFIRMED)
                {
                    /*A message transfer is started as soon as there is a write of the message object number to the MNUM
                    field when the TXRQST bit in the CANIF1MCTL register is set.*/

                    CAN0_IF1CRQ_R = CanMailBoxIndex + 1   ;

                    /*casting pointer to integral type unavoidable when addressing memory mapped
                                           registers or other hardware specific features.*/
                    /*This message object is not waiting for transmission if this bit is cleared  */
                    /*TODO:IS_CONFIRMED*/
                    if( ( CAN0_IF1MCTL_R & MESSAGE_WAITING_MASK)   != MESSAGE_WAITING_MASK )/*Message_Confirmation[i][j]=un_confirmed*/
                    {

                        Can_MailBoxLookUpTables[Controller][CanMailBoxIndex].Confirmation = CONFIRMED;
                        /*TODO: #if DEBUG == TRUE */
                        /*                        UARTprintf("MainFunction : MailBox =%u\n", CanMailBoxIndex +1);*/

                        /*TODO:#if */
                        CanIf_TxConfirmation(Can_MailBoxLookUpTables[Controller][CanMailBoxIndex].pduId);

                        /*[SWS_Can_00016] The Can module shall call CanIf_TxConfirmation to indicate a
                                      successful transmission.It shall either called by the TX-interrupt service routine
                                      of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                      polling mode.*/

                        /*7.8 Notification concept*/

                        /*CanDrv must store the all in HTHs pending L-PDU Ids in an array
                                        organized perTH to avoid new search of the L-PDU ID for call of
                                       CanIf_TxConfirmation().*/
                    }
                }
                else
                {

                }
            }
            else
            {

            }
        }/*End of For */
    }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }

#endif









/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_MainFunction_Read********************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/


#if (CAN_CONTROLLER_CAN_RX_PROCESSING == POLLING)

FUNC (void,CAN_CODE)  Can_MainFunction_Read(void)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                {
    VAR(uint32_t,AUTOMATIC) mailBoxIndex                  ;
    /*  Buffer to receive data in                                       */
    VAR(uint8_t,AUTOMATIC) rx_MsgData1[8U]={0U}           ;
    /*  TI struct to use in data reception      (Tivaware)              */
    VAR(tCANMsgObject,AUTOMATIC)  CANMsgObject;
    /*  Point at the buffer defined above                               */
    VAR(CANMsgObject,AUTOMATIC) .pui8MsgData = rx_MsgData1  ;
    /*  PduInfo is needed here for Canif_RxIndication which uses Pdu    */
    VAR(PduInfoType,AUTOMATIC)  rxPduInfo                 ;
    VAR(Can_HwType,AUTOMATIC)  Rx_Mailbox                 ;
    VAR(uint8_t,AUTOMATIC) Controller                     ;
    VAR(uint8_t,AUTOMATIC) HOIndex = START_INDEX          ;
    for ( Controller = CAN_CONTROLLER_ZERO ;Controller < MAX_NUM_CONTROLLER ;Controller++ )
    {
        /*TODO: Loop for the RX hwobjects */
        /*TODO: check if the cng var */
        for ( mailBoxIndex = START_INDEX ; mailBoxIndex < MAX_NUM_OF_HO; mailBoxIndex ++)
        {
            HOIndex = Can_MailBoxLookUpTables[Controller][mailBoxIndex].HwObject;
            if( CanHardwareObject[HOIndex].CanObjectType == RECEIVE )
            {
                /*This function shall take mailbox from 1 to 32 */
                CANMessageGet(CanController[Controller].CanControllerBaseAddress, mailBoxIndex + 1, &CANMsgObject, 0U);

                Rx_Mailbox.Ho            = HOIndex                ;
                Rx_Mailbox.CanId         = CANMsgObject.ui32MsgID ;
                Rx_Mailbox.controllerlId = Controller             ;
                /* [SWS_Can_00060] Data mapping by CAN to memory is defined in a way that the CAN data byte
                which is received first is array element 0 the CAN data byte which is received last is array element 7*/
                rxPduInfo.SduLength  = CANMsgObject.ui32MsgLen;
                rxPduInfo.SduDataPtr = CANMsgObject.pui8MsgData;

                CanIf_RxIndication(&Rx_Mailbox,&rxPduInfo);
            }
            else
            {

            }


        }
    }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }
#endif



/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************Can_MainFunction_Mode********************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/


FUNC(void,CAN_CODE) Can_MainFunction_Mode( void )
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                {

    //TOBEASKED: EH EL KALAM DA???
    /*[SWS_Can_00369]  The function Can_MainFunction_Mode shall implement the polling
        of CAN status register flags to detect transition of CAN Controller state. Compare to chapter 7.3.2.*/
    VAR(uint8_t,AUTOMATIC) Controller                                 ;
    LOCAL   Can_ControllerStateType PreviousState[MAX_NUM_CONTROLLER] = {CAN_CS_STOPPED} ;
    VAR(uint8_t,AUTOMATIC) RegisterCheck[MAX_NUM_CONTROLLER]                            ;
    for ( Controller = CAN_CONTROLLER_ZERO ; Controller < MAX_NUM_CONTROLLER ; Controller++)
    {
        if( state_transition_flag[Controller] == MODE_CHANGED )
        {
            if(Can_ControllerMode[Controller] != PreviousState[Controller])

            {
                state_transition_flag[Controller] = MODE_NOT_CHANGED                                                      ;
                /*  0 Normal operation  1 Initialization started    */
                RegisterCheck[ CAN_CONTROLLER_ZERO] = ( ( VAR(uint8_t,AUTOMATIC ) ) ( CAN0_CTL_R & CAN_CTL_R_INIT_BIT ) ) ;
                RegisterCheck[ CAN_CONTROLLER_ONE ] = ( ( VAR(uint8_t,AUTOMATIC ) ) ( CAN1_CTL_R & CAN_CTL_R_INIT_BIT ) ) ;

                if( Can_ControllerMode [Controller] == CAN_CS_STARTED && RegisterCheck[Controller] == NORMAL_MODE )
                {
                    PreviousState[Controller] = Can_ControllerMode[Controller];
                }
            }
            else if( Can_ControllerMode [Controller] == CAN_CS_STOPPED || Can_ControllerMode [Controller] == CAN_CS_SLEEP && RegisterCheck[Controller] == INITIALIZE_STARTED_MODE)
            {
                /*TOBEASKED: void CanIf_ControllerModeIndication()*/
                PreviousState[Controller] = Can_ControllerMode[ CAN_CONTROLLER_ZERO ];
            }
            else
            {

            }
        }
        else
        {
            /*No mode changed*/
        }
    }/*End of for*/

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }/*End of Can_MainFunction_Mode*/




/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/***************************************************CAN_IRQHandler***************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/




FUNC(void,CAN_CODE) CAN_IRQHandler(VAR(uint8_t, AUTOMATIC) Controller)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                {
    /* Only for development debugging */
    VAR(uint8_t,AUTOMATIC) ErrorStatus = E_OK;
    /* The mailBoxIndex return type from CANIntStatus  */
    VAR(uint32_t,AUTOMATIC) mailBoxIndex;
    /*  Buffer to recieve data in   */
    VAR(uint8_t,AUTOMATIC) rx_MsgData1[8U] = {0U};
    /*  TI struct to use in data reception      (Tivaware)*/
    /*  PduInfo is needed here for Canif_RxIndication which uses Pdu    */

    VAR(PduInfoType,AUTOMATIC)  rxPduInfo;
    VAR(Can_HwType,AUTOMATIC)  Rx_Mailbox;
    VAR(tCANMsgObject,AUTOMATIC)  CANMsgObject;
    /*  Point at the buffer defined above   */
    CANMsgObject.pui8MsgData = rx_MsgData1;
    /*
     *  This function returns a value upon each interrupt.
     *  If the value of the interrupt register is in the range 1-32, then this indicates the number of the
     *  highest priority message object that has an interrupt pending.
     *  Otherwise, there is an error.
     *  (tivaware)
     */

    mailBoxIndex = CANIntStatus(CanController[Controller].CanControllerBaseAddress, CAN_INT_STS_CAUSE);

    CANIntClear(CanController[Controller].CanControllerBaseAddress, mailBoxIndex)                     ;

    if( mailBoxIndex >= FIRST_MAIL_BOX && mailBoxIndex <= LAST_MAIL_BOX )
    {
        /* Can_MailBoxLookUpTables first index is zero while the first the Hw mailbox index is 1 therefore the index is decremented  */
        mailBoxIndex -=  1;
        VAR(uint8_t,AUTOMATIC) HwObject = Can_MailBoxLookUpTables[Controller][mailBoxIndex ].HwObject;

        if( CanHardwareObject[HwObject].CanObjectType == RECEIVE )
        {
            CANMessageGet( CanController[Controller].CanControllerBaseAddress, (uint32_t)mailBoxIndex +1 , &CANMsgObject, 0);
            Rx_Mailbox.Ho            = HwObject          ;
            Rx_Mailbox.CanId         = CANMsgObject.ui32MsgID ;
            Rx_Mailbox.controllerlId = Controller             ;

            /* [SWS_Can_00060] Data mapping by CAN to memory is defined in a way that the CAN data byte
            which is received first is array element 0 the CAN data byte which is received last is array element 7*/

            rxPduInfo.SduLength  = CANMsgObject.ui32MsgLen;
            rxPduInfo.SduDataPtr = CANMsgObject.pui8MsgData;
            //            UARTprintf("IRQ:Len=%d ,Data = %d ,%d\n",rxPduInfo.SduLength,CANMsgObject.pui8MsgData[0],CANMsgObject.pui8MsgData[1]);
            //            UARTprintf("IRQ:CanID=%d\n", Rx_Mailbox.CanId );
            /*  not here in the confirmation only   */
            CanIf_RxIndication(&Rx_Mailbox,&rxPduInfo);
        }
        else if ( CanHardwareObject[HwObject].CanObjectType == TRANSMIT )
        {
            Can_MailBoxLookUpTables[Controller][mailBoxIndex ].Confirmation = CONFIRMED;
            CanIf_TxConfirmation(Can_MailBoxLookUpTables[Controller][mailBoxIndex ].pduId);
        }
        else
        {

        }
    }
    else
    {
        ErrorStatus = E_NOT_OK ;
        /* ERROR */

    }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }


