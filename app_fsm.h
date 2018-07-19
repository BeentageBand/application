#ifndef APP_FSM_H_
#define APP_FSM_H_
#include "fsm.h"

#define APP_FSM(cb) \
FSM_STATE_DEF(cb, APP_ACTIVE_STID, \
FSM_TRANSITION_DEF(cb, APP_INT_START_THREADS_MID, APP_TERM_STID, App_FSM_Start_Thread) \
) \
FSM_STATE_DEF(cb, APP_TERM_STID, \
)

enum
{
    APP_ACTIVE_STID,
    APP_TERM_STID,
    APP_MAX_STID
};

extern struct FSM_Chart Application_St_Chart[APP_MAX_STID];

extern void App_FSM_Start_Thread(union State_Machine * const fsm);

#endif /*APP_FSM_H_*/