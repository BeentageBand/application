/*=====================================================================================*/
/**
 * app.h
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#ifndef APP_H_
#define APP_H_
#include "fsm.h"
#include "worker.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef union Application
{
    union Application_Class _private * _private vtbl;
    struct Object Object;
    union Worker Worker;
}Application_T;

typedef union Application_Class
{
    struct
    {
        union Worker_Class Worker;
        int (* _private startup)(union Application * const);
    };
    struct Class Class;
}Application_Class_T;

extern Application_Class_T _private Application_Class;

extern void Populate_Application(union Application * const this, union Worker * (* factory_method)(IPC_TID_T const tid));
extern void Application_initialized(void);
extern void Application_terminated(void);
extern void Application_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /*APP_H_*/