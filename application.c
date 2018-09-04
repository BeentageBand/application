/*=====================================================================================*/
/**
 * app.c
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/

#define Dbg_FID DBG_FID_DEF(APP_FID, 0)
#define COBJECT_IMPLEMENTATION
#include "application.h"
#include "application_fsm.h"
#include "dbg_log.h"
#include "ipc.h"

/*==============================================================================
 * Local Types
 * ============================================================================*/

/*==============================================================================
 * Local Prototypes
 * ============================================================================*/
static void application_delete(struct Object * const obj);
static void application_on_mail(union Worker * const super, union Mail * const mail);
static void application_on_start(union Worker * const super);
static void application_on_loop(union Worker * const super);
static void application_on_stop(union Worker * const super);
static int application_startup(union Application * const this);

/*==============================================================================
 * Local Objects 
 * ============================================================================*/
Application_Class_T Application_Class =
{
        { // Worker
                {
                        {application_delete, NULL},
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL
                },
                application_on_mail,
                application_on_start,
                application_on_loop,
                application_on_stop
        },
        application_startup
};

static union Application Application = {NULL};
static union Mail Application_Mail_Buff[64] ={0};
static union App_FSM Application_Pool[IPC_MAX_TID] = {0};

/*==============================================================================
 * Local Functions
 * ============================================================================*/
void application_delete(struct Object * const obj)
{
}

void application_on_mail(union Worker * const super, union Mail * const mail)
{
    union Application * const this = _cast(Application, super);
    Isnt_Nullptr(this, );
    Isnt_Nullptr(mail, );

    Dbg_Info("%s: mid = %d from tid = %d",
            __func__,
            mail->mid,
            mail->sender);

    if(mail->mid >= APP_INT_START_THREADS_MID &&
            mail->mid <= APP_INT_SHUTDOWN_MID)
    {
        Isnt_Nullptr(mail->payload, );
        IPC_TID_T tid = *(IPC_TID_T *) mail->payload;
        Dbg_Info("%s: tid %d mid %d", __func__, tid, mail->mid);

        union State_Machine * const st_m= &Application_Pool[tid].State_Machine;
        if(NULL == st_m->vtbl)
        {
            Dbg_Fault("%s: tid %s state machine not initialized??", __func__, tid);
        }
        else
        {
            st_m->vtbl->dispatch(st_m, mail);
        }
    }
}

void application_on_start(union Worker * const super)
{
    union Application * const this = _cast(Application, super);
    Isnt_Nullptr(this, );
}

void application_on_loop(union Worker * const super)
{
    bool is_alive = true;
    IPC_TID_T i;
    for(i = 0; i < IPC_MAX_TID; ++i)
    {
        if(Application_Pool[i].worker &&
                APP_TERM_STID == Application_Pool[i].State_Machine.current_st)
        {
            Dbg_Warn("%s: %d thread is APP_TERM_STID", __func__, i);
            is_alive = false;
            break;
        }
    }

    if(!is_alive)
    {
        IPC_Send_Self(WORKER_INT_SHUTDOWN_MID, NULL, 0);
    }

    IPC_Sleep(500);
    Dbg_Info("%s: is alive", __func__);
}

void application_on_stop(union Worker * const super)
{
    union Application * const this = _cast(Application, super);
    Isnt_Nullptr(this, );
}

int application_startup(union Application * const this)
{
    IPC_TID_T i;
    union State_Machine * st_m;
    for(i = 0; i < IPC_MAX_TID; ++i)
    {
        union Worker * const worker = Application_Pool[i].worker;
        st_m = &Application_Pool[i].State_Machine;
        if(NULL != worker)
        {
            Dbg_Info("%s: send start to tid = %d",
                    __func__,
                    i);
            IPC_Send(APP_WORKER_TID, APP_INT_THREAD_INIT_MID, &i, sizeof(i));
            st_m->vtbl->dispatch(st_m, NULL);
        }
    }
    st_m = &Application_Pool[APP_WORKER_TID].State_Machine;

    Isnt_Nullptr(st_m, -1);

    union Mail mail = {NULL};
    i = APP_WORKER_TID;
    Populate_Mail(&mail, APP_INT_THREAD_INIT_MID,
            APP_WORKER_TID, APP_WORKER_TID, &i, sizeof(i));
    st_m->vtbl->dispatch(st_m, &mail);
    _delete(&mail);
    while(1){}
    return -1; //TERMINATED
}

/*==============================================================================
 * External Functions
 * ============================================================================*/
void Populate_Application(union Application * const this, union Worker * (* const factory_method)(IPC_TID_T const tid))
{
    if(NULL == Application.vtbl)
    {
        Populate_Worker(&Application.Worker, APP_WORKER_TID,
                Application_Mail_Buff, Num_Elems(Application_Mail_Buff));
        Object_Init(&Application.Object, &Application_Class.Class,
                sizeof(Application_Class.Worker));
        Application.vtbl = &Application_Class;
        Application_Class.Worker.on_start = application_on_start;
        Application_Class.Worker.on_loop = application_on_loop;
        Application_Class.Worker.on_stop = application_on_stop;
        Application_Class.Worker.on_mail = application_on_mail;
    }
    _clone(this, Application);

    Populate_App_FSM(Application_Pool + APP_WORKER_TID, &this->Worker);
    IPC_TID_T i;
    for(i = 0; i < IPC_MAX_TID; ++i)
    {
        union Worker * worker = factory_method(i);
        if(NULL != worker)
        {
            Populate_App_FSM(Application_Pool + i,
                    worker);
        }
    }
}

void Application_initialized(void)
{
    IPC_TID_T self = IPC_Self();
    Dbg_Info("%s: tid  %d", __func__, self);
    IPC_Send(APP_WORKER_TID, APP_INT_THREAD_INIT_MID, &self, sizeof(self));
}

void Application_terminated(void)
{
    IPC_TID_T self = IPC_Self();
    IPC_Send(APP_WORKER_TID, APP_INT_THREAD_TERM_MID, &self, sizeof(self));
    Dbg_Info("%s: tid  %d", __func__, self);
}

void Application_shutdown(void)
{
    IPC_TID_T self = IPC_Self();
    IPC_Send(APP_WORKER_TID,  APP_INT_SHUTDOWN_MID, &self, sizeof(self));
    Dbg_Warn("%s: tid  %d", __func__, self);
}
