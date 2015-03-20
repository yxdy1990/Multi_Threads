/*
 * =====================================================================================
 *       Filename:  msg_send.c
 *    Description:  
 *        Version:  1.0
 *        Created:  03/20/2015 03:34:32 PM
 *       Revision:  none
 *         Author:  Evan Wang, Evan.Wang@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 * =====================================================================================
 */
#include "common.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Main
 *  Description:  Main Function
 * =====================================================================================
 */
int main ( int argc, char *argv[] )			
{
    int msq_id;
    int msg_flg = IPC_CREAT | 0666;
    key_t key;
    st_msg_buf msg_buf;
    size_t buf_size;

    /*
     * Get the message queue id for the "name" 1234, which was created by the server.
     */
    key = 1234;

    (void) fprintf(stdout, "\nmsgget: Calling msgget(%#lx, %#o)\n", key, msg_flg);
    if ((msq_id = msgget(key, msg_flg)) < 0) 
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    else 
    {
        (void) fprintf(stdout,"msgget: msgget succeeded: msq_id = %d\n", msq_id);
        /*
         * We'll send message type 1
         */            
        msg_buf._lType = 1;                        
        (void) strcpy(msg_buf._cText, "Did you get this?");                  
        buf_size = strlen(msg_buf._cText) + 1;
        /*
         * Send a message.
         */
        if (msgsnd(msq_id, &msg_buf, buf_size, IPC_NOWAIT) < 0)
        {
            perror("msgsnd");
            printf ("%d, %d, %s, %d\n", msq_id, msg_buf._lType, msg_buf._cText, buf_size);
            exit(EXIT_FAILURE);
        }
        else 
        {
            printf("Message Sent: \\%s\\ \n", msg_buf._cText);                                                         
        }
    }

    return EXIT_SUCCESS;
}
/* ----------  end of function main  ---------- */

