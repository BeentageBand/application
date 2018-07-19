/*=====================================================================================*/
/**
 * app_evs.h
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
#ifndef APP_EVS_H_
#define APP_EVS_H_

#define APP_INT_MAILIST(X) \
   X(APP_INT_START_THREADS_MID, "Start fsm for worker") \
   X(APP_INT_THREAD_INIT_MID,     "MAILS TO SCHEDULER THAT TASK IS INITIATED") \
   X(APP_INT_THREAD_TERM_MID,     "MAILS TO SCHEDULER THAT TASK IS TERMINATED") \
   X(APP_INT_SHUTDOWN_MID,      "MAILS TO SCHEDULER TO SHUTDOWN ALL APPS") \

#define APP_PBC_MAILIST(X) \

#endif /*APP_EVS_H_*/