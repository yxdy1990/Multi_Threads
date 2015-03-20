/*
 * =====================================================================================
 *       Filename:  msg_recv.c
 *    Description:  none
 *        Version:  1.0
 *        Created:  03/20/2015 04:23:19 PM
 *       Revision:  none
 *         Author:  Evan Wang, Evan.Wang@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 * =====================================================================================
 */
#include	"common.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Main
 *  Description:  Main Function
 * =====================================================================================
 */

int main ( int argc, char *argv[] )
{
    int msq_id;
    key_t key;
    st_msg_buf msg_buf;

    key = 1234;

    (void) fprintf(stdout, "\nCalling msgget\n");
    if ((msq_id = msgget(key, 0666)) < 0)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    else
    {
        /* Receive message from queue */
        if (msgrcv(msq_id, &msg_buf, MSGSIZE, 1, 0) < 0)
        {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Message received: \\%s\\ \n", msg_buf._cText);
        }
    }

    return EXIT_SUCCESS;
}
/* ----------  end of function main  ---------- */

