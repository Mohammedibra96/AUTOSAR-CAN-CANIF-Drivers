#include"include/Std_Types.h"
#include"include/Det.h"



void Det_ReportError(uint16_t ModuleId ,uint8_t Instanceid ,uint8_t ApiId ,uint8_t ErrorId)
{

    UARTprintf("Module=%x,ApiId=%x,error=%x",ModuleId,ApiId,ErrorId);
    while(1)
    {
    }
}
