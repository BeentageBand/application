#ifndef APPLICATION_FSM_H_
#define APPLICATION_FSM_H_

#include "application_types.h"
#include "fsm.h"
#include "worker.h"

#define App_FSM_Do_Nothing NULL

#define APP_FSM(cb) \
FSM_STATE_DEF(cb, APP_NOT_RUNNING_STID, \
    FSM_TRANSITION_DEF(cb, APP_INT_THREAD_INIT_MID,       APP_IDLE_STID,   APP_NOT_RUNNING_guard, App_FSM_Start_Thread) \
) \
FSM_STATE_DEF(cb, APP_IDLE_STID, \
    FSM_TRANSITION_DEF(cb, APP_INT_THREAD_ACTIVE_REQ_MID, APP_ACTIVE_STID, APP_IDLE_guard,        App_FSM_Activate_Thread) \
    FSM_TRANSITION_DEF(cb, APP_INT_THREAD_TERM_MID,       APP_TERM_STID,   APP_IDLE_guard,        App_FSM_Term_Thread) \
) \
FSM_STATE_DEF(cb, APP_ACTIVE_STID, \
    FSM_TRANSITION_DEF(cb, APP_INT_THREAD_IDLE_REQ_MID,   APP_IDLE_STID,   APP_ACTIVE_guard,      App_FSM_Idle_Thread) \
    FSM_TRANSITION_DEF(cb, APP_INT_THREAD_TERM_MID,       APP_TERM_STID,   APP_ACTIVE_guard,      App_FSM_Term_Thread) \
) \
FSM_STATE_DEF(cb, APP_TERM_STID, \
    FSM_TRANSITION_DEF(cb, APP_INT_THREAD_TERM_MID,       APP_TERM_STID,   APP_TERM_guard,        App_FSM_Do_Nothing) \
)


#ifdef __cplusplus
extern "C"{
#endif

typedef union App_FSM
{
	union App_Class _private * _private vtbl;
	struct Object Object;
	union State_Machine State_Machine;
	struct
	{
		union FSM FSM;
		union Worker _private * _private worker;
	};
}App_FSM_T;

typedef union App_FSM_Class
{
	struct Class Class;
	struct State_Machine_Class State_Machine;
	union FSM_Class FSM;
}App_FSM_Class_T;

extern union App_FSM_Class _private App_FSM_Class;

extern void Populate_App_FSM(union App_FSM * const app_fsm, union Worker * const worker);

#ifdef __cplusplus
}
#endif
#endif /*APPLICATION_FSM_H_*/
