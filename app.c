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
#include "dbg_log.h"
#include "ipc.h"
#include "app.h"
#include "app_ext.h"

/*==============================================================================
 * Local Prototypes
 * ============================================================================*/
static void application_delete(struct Object * const obj);
static void application_on_mail(union Worker * const super, union Mail * const mail);
static void application_on_start(union Worker * const super);
static void application_on_loop(union Worker * const super);
static void application_on_stop(union Worker * const super);
static void application_startup(union Application * const this);

/*==============================================================================
 * Local Objects 
 * ============================================================================*/
Application_Class_T Application_Class =
{{
      {
            {application_delete, NULL},
            application_on_mail,
            application_start, 
            application_loop,  
            application_stop
      },
      application_startup
}};

static union Application Application = {{NULL}};
static union Mailbox Application_Mailbox = {{NULL}};
static union Mail Application_Mail_Buff[64] ={0};
static union FSM Application_Pool[IPC_MAX_TID] = {0};

/*==============================================================================
 * Local Functions
 * ============================================================================*/
void application_delete(struct Object * const obj)
{
   Application_T * const this = _cast(Application, obj);
   Isnt_Nullptr(this, );
}

void application_on_mail(union Worker * const super, union Mail * const mail)
{

}

void application_on_start(union Worker * const super)
{
   Application_T * const this = _cast(Application, super);
   Isnt_Nullptr(this, );
}

void application_on_loop(union Worker * const super)
{
   Application_T * const this = _dynamic_cast(Application, super);
   Isnt_Nullptr(this, );
   Mail_T const mail = {{NULL}};
   if (IPC_retreive_mail(&mail, IPC_RETRIEVE_TOUT_MS))
   {
      if(mail.tid > APP_START_THREADS_INT_MID)
      {
            /* */
      }

   }
}

void application_on_stop(union Worker * const super)
{
   Application_T * const this = _dynamic_cast(Application, super);
   Isnt_Nullptr(this, );
   Hama_HSM_Event_T ev =
   {
         0, NULL, 0 /* FIXME terminal */
   };
   this->fsm.vtbl->dispatch(&this->fsm, &ev);
}

void application_startup(union Application * const this)
{
      IPC_Run(APP_WORKER_TID);
}

/*==============================================================================
 * External Functions
 * ============================================================================*/
void Populate_Application(union Application * const this, union Worker * (* factory_method)(IPC_TID_T const tid))
{
      if(NULL == Application.vtbl)
      {
            Populate_Worker(&Application.Worker, APP_WORKER_TID, &Application_Mailbox, 
            Application_Mail_Buff, Num_Elems(Application_Mail_Buff));
            Object_Init(&Application.Object, &Application_Class.Class,
            sizeof(Application_Class.Worker));
            Application.vtbl = &Application_Class;
            Application.fsm = Application_Pool;
            Application.fsm = Num_Elems(Application_Pool)
      }
      memcpy(this, &Application, sizeof(Application));

      IPC_TID_T i;
      for(i = 0; i < IPC_MAX_TID; ++i)
      {
            Application_Pool[i].worker = factory_method(i);
            if(NULL != Application_Pool[i].worker)
            {
                  Populate_FSM(&Application_Pool[i].fsm, 
                  Application_St_Chart, Num_Elems(Application_St_Chart),
                  Application_St_Machine, Num_Elems(Application_St_Machine));
            }
      }
}

void Application_run_all_apps(void)
{
   Application_T * this = NULL;
   Application_get_instance(&this);
   Isnt_Nullptr(this,);
   this->Worker.Task.vtbl->run(&this->Worker.Task);

}

void Application_initialized(void)
{
   IPC_send(APP_WORKER_TID,  APP_TASK_INIT_TID, NULL, 0);
}

void Application_terminated(void)
{
   IPC_send(APP_WORKER_TID,  APP_TASK_TERM_TID, NULL, 0);
}

void Application_shutdown(void)
{
   IPC_send(APP_WORKER_TID,  APP_SHUTDOWN_TID, NULL, 0);
}