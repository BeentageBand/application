#define COBJECT_IMPLEMENTATION
#define Dbg_FID APP_FID, 2
#include "application_fsm.h"
#include "dbg_log.h"
#include "ipc.h"

static void app_fsm_delete(struct Object * const obj);
static bool APP_NOT_RUNNING_guard(union State_Machine * const fsm, union St_Machine_State * const state);
static bool APP_IDLE_guard(union State_Machine * const fsm, union St_Machine_State * const state);
static bool APP_ACTIVE_guard(union State_Machine * const fsm, union St_Machine_State * const state);
static bool APP_TERM_guard(union State_Machine * const fsm, union St_Machine_State * const state);
static void App_FSM_Start_Thread(union State_Machine * const fsm);
static void App_FSM_Activate_Thread(union State_Machine * const fsm);
static void App_FSM_Idle_Thread(union State_Machine * const fsm);
static void App_FSM_Term_Thread(union State_Machine * const fsm);


FSM_Declare_Chart(APP_FSM, Application_St_Chart)
static union App_FSM App_FSM = {NULL};
static union St_Machine_State Application_St_Machine[APP_MAX_STID];

union App_FSM_Class App_FSM_Class =
{
        {{app_fsm_delete, NULL}}
};

void app_fsm_delete(struct Object * const obj)
{}

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
    union App_FSM * const this = _cast(App_FSM, fsm);
    Isnt_Nullptr(this, );
    Isnt_Nullptr(this->worker, );
    IPC_Run(this->worker->Thread.tid);
    Dbg_Info("%s:start thread %d", __func__, this->worker->Thread.tid);
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

void Populate_App_FSM(union App_FSM * const this, union Worker * const worker)
{
    if(NULL == App_FSM.vtbl)
    {
        Populate_FSM(&App_FSM.FSM,
                Application_St_Chart, Num_Elems(Application_St_Chart),
                Application_St_Machine, Num_Elems(Application_St_Machine));
        Object_Init(&App_FSM.Object,
                &App_FSM_Class.Class,
                sizeof(App_FSM_Class.FSM));
    }
    _clone(this, App_FSM);
    this->worker = worker;
}
