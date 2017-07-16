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
#define CLASS_IMPLEMENTATION
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "dbg_log.h"
#include "ipc.h"
#include "app.h"
#include "app_ext.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/

/*=====================================================================================* 
 * Local X-Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Define Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Type Definitions
 *=====================================================================================*/
struct App_Handler_T
{
   IPC_Mail_Id_T mail_id;
   void (* process)(App_T * const, void const * const data, size_t const data_size);
};
/*=====================================================================================* 
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEFINITION
static App_T * Single_App = NULL;
static struct App_Handler_T App_Handler[] =
{

};
/*=====================================================================================* 
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/
static int App_Handler_Compare(void const * a, void const * b);
static void App_Ctor(App_T * const this, IPC_Process_Id_T const pid);
void App_on_loop(Worker_T * const super);
void App_on_start(Worker_T * const super);
void App_on_stop(Worker_T * const super);
/*=====================================================================================* 
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Definitions
 *=====================================================================================*/
void App_init(void)
{
   printf("%s \n", __FUNCTION__);
   App_Obj.Worker = Worker();

   App_Vtbl.Worker.Task.Object.rtti = &App_Rtti;
   App_Vtbl.Worker.Task.Object.destroy = App_Dtor;
   App_Vtbl.Worker.on_start = App_on_start;
   App_Vtbl.Worker.on_loop = App_on_loop;
   App_Vtbl.Worker.on_stop = App_on_stop;
   App_Vtbl.ctor = App_Ctor;

}

void App_shut(void) {}

void App_Dtor(Object_T * const obj)
{
   App_T * const this = _dynamic_cast(App, obj);
   Isnt_Nullptr(this, );
}

void App_on_loop(Worker_T * const super)
{
   App_T * const this = _dynamic_cast(App, super);
   Isnt_Nullptr(this, );
   Mail_T * const mail = IPC_retreive_mail(IPC_RETRIEVE_TOUT_MS);
   Isnt_Nullptr(mail, );
   struct App_Handler_T * handle = bsearch(&mail->mail_id, App_Handler,
         Num_Elems(App_Handler), sizeof(*App_Handler),
         App_Handler_Compare);

   Isnt_Nullptr(handle, );
   handle->process(this, mail->data, mail->data_size);
}

void App_on_start(Worker_T * const super)
{
   App_T * const this = _dynamic_cast(App, super);
   Isnt_Nullptr(this, );
   this->hsm.vtbl->ctor(&this->hsm, 0, NULL, 0, NULL, 0); /* FIXME initialize */
}

void App_on_stop(Worker_T * const super)
{
   App_T * const this = _dynamic_cast(App, super);
   Isnt_Nullptr(this, );
   Hama_HSM_Event_T ev =
   {
         0, NULL, 0 /* FIXME terminal */
   };
   this->hsm.vtbl->dispatch(&this->hsm, &ev);
}

int App_Handler_Compare(void const * a, void const * b)
{
   IPC_Mail_Id_T const * ida = (IPC_Mail_Id_T const *)a;
   IPC_Mail_Id_T const * idb = (IPC_Mail_Id_T const *)b;
   return (*ida - *idb);
}
/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
void App_Ctor(App_T * const this, IPC_Process_Id_T const pid)
{
   this->vtbl->Worker.ctor(&this->Worker, HAMA_SCHED_WORKER, pid);
}

void App_run_all_apps(void)
{
   App_T * this = NULL;
   App_get_instance(&this);
   Isnt_Nullptr(this,);
   this->Worker.Task.vtbl->run(&this->Worker.Task);

}

void App_initialized(void)
{
   IPC_send(HAMA_SCHED_WORKER, HAMA_SCHED_PROCESS, HAMA_SCHED_TASK_INIT, NULL, 0);

}

void App_terminated(void)
{
   IPC_send(HAMA_SCHED_WORKER, HAMA_SCHED_PROCESS, HAMA_SCHED_TASK_TERM, NULL, 0);

}

void App_shutdown(void)
{
   IPC_send(HAMA_SCHED_WORKER, HAMA_SCHED_PROCESS, WORKER_SHUTDOWN, NULL, 0);
}
/*=====================================================================================* 
 * app.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
