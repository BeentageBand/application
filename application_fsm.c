#include "application_fsm.h"

FSM_Declare_Chart(APP_FSM, Application_St_Chart)

bool APP_NOT_RUNNING_STID_guard(union State_Machine * const fsm, union St_Machine_State * const state)
{
    return true;
}

bool APP_IDLE_STID_guard(union State_Machine * const fsm, union St_Machine_State * const state)
{
    return true;
}

bool APP_ACTIVE_STID_guard(union State_Machine * const fsm, union St_Machine_State * const state)
{
    return true;
}

bool APP_TERM_STID_guard(union State_Machine * const fsm, union St_Machine_State * const state)
{
    return true;
}

void App_FSM_Start_Thread(union State_Machine * const fsm)
{
}
void App_FSM_Activate_Thread(union State_Machine * const fsm)
{

}

void App_FSM_Idle_Thread(union State_Machine * const fsm)
{

}
void App_FSM_Term_Thread(union State_Machine * const fsm)
{

}