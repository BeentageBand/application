/*=====================================================================================*/
/**
 * arduino_fwk.cpp
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
#include "hama_dbg_trace.h"
#include "ipc.h"
#include "hamatora_sched.h"
#include "hamatora_sched_ext.h"
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
struct Hama_Sched_Handler_T
{
   IPC_Mail_Id_T mail_id;
   void (* process)(Hama_Sched_T * const, void const * const data, size_t const data_size);
};
/*=====================================================================================* 
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEFINITION
static Hama_Sched_T * Single_Hama_Sched = NULL;
static struct Hama_Sched_Handler_T Hama_Sched_Handler[] =
{

};
/*=====================================================================================* 
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/
static int Hama_Sched_Handler_Compare(void const * a, void const * b);
static void Hama_Sched_Ctor(Hama_Sched_T * const this, IPC_Process_Id_T const pid);
void Hama_Sched_on_loop(Worker_T * const super);
void Hama_Sched_on_start(Worker_T * const super);
void Hama_Sched_on_stop(Worker_T * const super);
/*=====================================================================================* 
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Definitions
 *=====================================================================================*/
void Hama_Sched_init(void)
{
   printf("%s \n", __FUNCTION__);
   Hama_Sched_Obj.Worker = Worker();

   Hama_Sched_Vtbl.Worker.Task.Object.rtti = &Hama_Sched_Rtti;
   Hama_Sched_Vtbl.Worker.Task.Object.destroy = Hama_Sched_Dtor;
   Hama_Sched_Vtbl.Worker.on_start = Hama_Sched_on_start;
   Hama_Sched_Vtbl.Worker.on_loop = Hama_Sched_on_loop;
   Hama_Sched_Vtbl.Worker.on_stop = Hama_Sched_on_stop;
   Hama_Sched_Vtbl.ctor = Hama_Sched_Ctor;

}

void Hama_Sched_shut(void) {}

void Hama_Sched_Dtor(Object_T * const obj)
{
   Hama_Sched_T * const this = _dynamic_cast(Hama_Sched, obj);
   Isnt_Nullptr(this, );
}

void Hama_Sched_on_loop(Worker_T * const super)
{
   Hama_Sched_T * const this = _dynamic_cast(Hama_Sched, super);
   Isnt_Nullptr(this, );
   Mail_T * const mail = IPC_retreive_mail(IPC_RETRIEVE_TOUT_MS);
   Isnt_Nullptr(mail, );
   struct Hama_Sched_Handler_T * handle = bsearch(&mail->mail_id, Hama_Sched_Handler,
         Num_Elems(Hama_Sched_Handler), sizeof(*Hama_Sched_Handler),
         Hama_Sched_Handler_Compare);

   Isnt_Nullptr(handle, );
   handle->process(this, mail->data, mail->data_size);
}

void Hama_Sched_on_start(Worker_T * const super)
{
   Hama_Sched_T * const this = _dynamic_cast(Hama_Sched, super);
   Isnt_Nullptr(this, );
   this->hsm.vtbl->ctor(&this->hsm, 0, NULL, 0, NULL, 0); /* FIXME initialize */
}

void Hama_Sched_on_stop(Worker_T * const super)
{
   Hama_Sched_T * const this = _dynamic_cast(Hama_Sched, super);
   Isnt_Nullptr(this, );
   Hama_HSM_Event_T ev =
   {
         0, NULL, 0 /* FIXME terminal */
   };
   this->hsm.vtbl->dispatch(&this->hsm, &ev);
}

int Hama_Sched_Handler_Compare(void const * a, void const * b)
{
   IPC_Mail_Id_T const * ida = (IPC_Mail_Id_T const *)a;
   IPC_Mail_Id_T const * idb = (IPC_Mail_Id_T const *)b;
   return (*ida - *idb);
}
/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
void Hama_Sched_Ctor(Hama_Sched_T * const this, IPC_Process_Id_T const pid)
{
   this->vtbl->Worker.ctor(&this->Worker, HAMA_SCHED_WORKER, pid);
}

void Hama_Sched_run_all_apps(void)
{
   Hama_Sched_T * this = NULL;
   Hama_Sched_get_instance(&this);
   Isnt_Nullptr(this,);
   this->Worker.Task.vtbl->run(&this->Worker.Task);

}

void Hama_Sched_initialized(void)
{
   IPC_send(HAMA_SCHED_WORKER, HAMA_SCHED_PROCESS, HAMA_SCHED_TASK_INIT, NULL, 0);

}

void Hama_Sched_terminated(void)
{
   IPC_send(HAMA_SCHED_WORKER, HAMA_SCHED_PROCESS, HAMA_SCHED_TASK_TERM, NULL, 0);

}

void Hama_Sched_shutdown(void)
{
   IPC_send(HAMA_SCHED_WORKER, HAMA_SCHED_PROCESS, WORKER_SHUTDOWN, NULL, 0);
}
/*=====================================================================================* 
 * arduino_fwk.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
