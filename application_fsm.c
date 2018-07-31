#include "application_fsm.h"

static bool APP_NOT_RUNNING_guard(union State_Machine * const fsm, union St_Machine_State * const state);
static bool APP_IDLE_guard(union State_Machine * const fsm, union St_Machine_State * const state);
static bool APP_ACTIVE_guard(union State_Machine * const fsm, union St_Machine_State * const state);
static bool APP_TERM_guard(union State_Machine * const fsm, union St_Machine_State * const state);

FSM_Declare_Chart(APP_FSM, Application_St_Chart)

bool APP_NOT_RUNNING_guard(union State_Machine * const fsm, union St_Machine_State * const state)
{
    return true;
}

bool APP_IDLE_guard(union State_Machine * const fsm, union St_Machine_State * const state)
{
    return true;
}

bool APP_ACTIVE_guard(union State_Machine * const fsm, union St_Machine_State * const state)
{
    return true;
}

bool APP_TERM_guard(union State_Machine * const fsm, union St_Machine_State * const state)
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