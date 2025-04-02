#include "app_OPTO_switch.h"
#include "app_sonar.h"
#include "stdbool.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_visual_identity.h"



extern xTaskHandle ServoControlTaskHandle;
extern xTaskHandle StepMotorTaskHandle;
int state0 ;
int state1 =1 ;//系统开关状态,决定是否运行
int state2 =0;

int pre_state1=0;

int count = 0;
//**检测状态**
extern bool rubbish_is_detected;
extern int infrared_state1 ;
extern int reflective_state1 ;
extern int reflective_state2 ;
extern int reflective_state3 ;
extern int reflective_state4 ;
extern int current_rubbish_type;
extern int rubbish_type ;         // 实时检测
extern int full_state ;


void OPTOswitchReadTask(void* argument)
{
    for(;;)
    {

        OPTOswitch_Proc();

    }
}

void OPTOswitch_Proc()
{
    //通知步进电机启动的判断逻辑
    if((reflective_state1 ==1 ||state1 == 1))
    {
        if(state1 != 1) state1 = 1;

        if(pre_state1==0) xTaskNotifyGive(StepMotorTaskHandle);

    }
    pre_state1=state1;//记录上一次state1状态




    // //通知舵机启动的判断逻辑
    // if(current_rubbish_type!=RUBBISH_TYPE_NONE)
    // {
    //     xTaskNotifyGive(ServoControlTaskHandle);
    // }


}
