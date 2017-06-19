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
#include "hamatora_sched_def.h"
#include "hamatora_sched_ext.h"
#include "hama_dbg_trace.h"
#include "hamatora_sched.h"
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
static bool Stop_Hama_Sched = true;

#if HOST
#undef HAMA_SCHED_APP
#define HAMA_SCHED_APP(app, init, run, stop) const Pgm_Char_T Hama_Sched_##app[] PROGMEM = #app;

HAMA_SCHED_APPS_TABLE

#undef HAMA_SCHED_APP
#define HAMA_SCHED_APP(app, init, run, stop) Hama_Sched_##app,

const Pgm_Char_T * const Sched_Apps_Names[] PROGMEM =
{
      HAMA_SCHED_APPS_TABLE
};

#endif


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
   bool_t is_unregistered = Hama_Sched_unregister_process(this);
}
void Hama_Sched_on_loop(Worker_T * const super)
{
   while (Stop_Hama_Sched)
   {
      for(uint8_t app_id = 0; app_id < pgm_read_byte(Num_Of_Scheduled_Apps); app_id++)
      {
         if(0 != Scheduled_Apps[app_id].run)
         {
#if HOST
            TR_INFO_1("Main - %s", Sched_Apps_Names[app_id]);
#else
            TR_INFO_1("Main ", app_id);
#endif
            Scheduled_Apps[app_id].run();
         }
         else
         {
            TR_INFO("Null run");
         }
      }
   }
}

static void Hama_Sched_on_start(void)
{
   for(uint8_t app_id = 0; app_id < pgm_read_byte(Num_Of_Scheduled_Apps); app_id++)
   {
      if(0 != Scheduled_Apps[app_id].init)
      {
#if HOST
         TR_INFO_1("Init - %s", Sched_Apps_Names[app_id]);
#else
         TR_INFO_1("Init - ", ((int)app_id) );
#endif
         Scheduled_Apps[app_id].init();
      }
      else
      {
         TR_INFO("Null Init");
      }
   }
}

static void Hama_Sched_on_stop(void)
{
   for(uint8_t app_id = 0; app_id < pgm_read_byte(Num_Of_Scheduled_Apps); app_id++)
   {
      if(0 != Scheduled_Apps[app_id].stop)
      {
#if HOST
         TR_INFO_1("Stop - %s", Sched_Apps_Names[app_id]);
#else
         TR_INFO_1("Stop - ", app_id);
#endif
         Scheduled_Apps[app_id].stop();
      }
      else
      {
         TR_INFO("NULL Stop");
      }
   }
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

void Hama_Sched_shut(void)
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
