/*
 * =====================================================================================
 *       Filename:  common.h
 *    Description:  none
 *        Version:  1.0
 *        Created:  03/20/2015 04:27:14 PM
 *       Revision:  none
 *         Author:  Evan Wang, Evan.Wang@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 * =====================================================================================
 */
#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <net/if.h>
#include <signal.h>
//#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
 
#define MSGSIZE   128
#define MSG_QUEUE_KEY 1234
 
#define PROC_SEND   1
#define PROC_RECV   2
#define PROC_INVALID    0xff

/*
 * Declare the message structure.
 */

typedef struct {
    long _lType;
    int  _iSrc;
    char _cText[MSGSIZE];
}st_msg_buf;

#endif
