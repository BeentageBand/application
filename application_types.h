#ifndef APPLICATION_TYPES_H_
#define APPLICATION_TYPES_H_
#include "std_reuse.h"

#ifdef __cplusplus
extern "C"{
#endif
enum APP_STAT
{
    APP_STAT_OK = 0,
    APP_STAT_UKNOWN_FAILURE
};

typedef uint8_t App_Stat_T;

enum APP_STID
{
    APP_NOT_RUNNING_STID,
    APP_IDLE_STID,
    APP_ACTIVE_STID,
    APP_TERM_STID,
    APP_MAX_STID
};

#ifdef __cplusplus
}
#endif
#endif /*APPLICATION_TYPES_H_*/