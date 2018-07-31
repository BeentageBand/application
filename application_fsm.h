#ifndef APPLICATION_FSM_H_
#define APPLICATION_FSM_H_

#include "application_types.h"
#include "fsm.h"

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

extern struct FSM_Chart Application_St_Chart[APP_MAX_STID];

extern void App_FSM_Start_Thread(union State_Machine * const fsm);
extern void App_FSM_Activate_Thread(union State_Machine * const fsm);
extern void App_FSM_Idle_Thread(union State_Machine * const fsm);
extern void App_FSM_Term_Thread(union State_Machine * const fsm);


#ifdef __cplusplus
}
#endif
#endif /*APPLICATION_FSM_H_*/