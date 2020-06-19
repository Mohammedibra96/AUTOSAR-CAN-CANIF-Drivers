#include <ComStack_Types.h>
#include <Std_Types.h>
#include"stdint.h"
 #ifndef CanTp_CanIf
 #define CanTp_CanIf 
// if value of any index equals 0 is not used 
// it will not be accesed in the run time 
  
 #define cantp_canif_tx  \ 
   0,\
   22
           
 #define CanTpTxFcNPdu_canif  \ 
     4,\
     3
           
 #endif 
