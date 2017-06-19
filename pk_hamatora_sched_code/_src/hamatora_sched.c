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

/*=====================================================================================* 
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEFINITION
static Hama_Sched_T * Single_Hama_Sched = NULL;
/*=====================================================================================* 
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/
static void Hama_Sched_get_instance(Hama_Sched_T ** instance);
static void Hama_Sched_Ctor(Hama_Sched_T * const this, IPC_Process_Id_T const pid);
static void Hama_Sched_run(Hama_Sched_T * const this);
static void Hama_Sched_unlock(Hama_Sched_T * const this);
static void Hama_Sched_lock(Hama_Sched_T * const this);
static void Hama_Sched_shut(Hama_Sched_T * const this);
static void Hama_Sched_on_loop(Worker_T * const super);
static void Hama_Sched_on_start(Worker_T * const super);
static void Hama_Sched_on_stop(Worker_T * const super);
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
   Hama_Sched_Vtbl.ctor = Hama_Sched_Ctor;
   Hama_Sched_Vtbl.run = NULL;

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
	this->hsm->
}

static void Hama_Sched_on_start(void)
{

}

static void Hama_Sched_on_stop(void)
{

}
/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
void Hama_Sched_Ctor(Hama_Sched_T * const this, IPC_Process_Id_T const pid)
{
   this->vtbl->Worker.ctor(&this->Worker, 0, pid);
}
void Hama_Sched_run(Hama_Sched_T * const this)
{

}
void Hama_Sched_unlock(Hama_Sched_T * const this)
{

}

void Hama_Sched_lock(Hama_Sched_T * const this)
{

}

void Hama_Sched_shut(Hama_Sched_T * const this)
{

}

void Hama_Sched_run_all_apps(void)
{
   Hama_Sched_T * sched = NULL;
   Hama_Sched_get_instance(&sched);
   Isnt_Nullptr(sched,)
   sched->vtbl->run(sched);
}

void Hama_Sched_initialized(void)
{
	IPC_send(HAMA_SCHED_WORKER, HAMA_SCHED_PID, HAMA_SCHED_INIT, NULL, 0);
}

void Hama_Sched_terminated(void)
{
	IPC_send(HAMA_SCHED_WORKER, HAMA_SCHED_PID, HAMA_SCHED_TERM, NULL, 0);
}

void Hama_Sched_shutdown(void)
{
   Hama_Sched_T * sched = NULL;
   Hama_Sched_get_instance(&sched);
   Isnt_Nullptr(sched,)
   sched->vtbl->shut(sched);
}
/*=====================================================================================* 
 * arduino_fwk.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
