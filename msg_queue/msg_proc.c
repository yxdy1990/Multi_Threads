/*
 * =====================================================================================
 *       Filename:  msg_proc.c
 *    Description:  Create two processes, one for send stuff, the other for receive.
 *        Version:  1.0
 *        Created:  03/23/2015 08:46:02 AM
 *       Revision:  none
 *         Author:  Evan Wang, Evan.Wang@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 * =====================================================================================
 */
#include    "common.h"



/*-----------------------------------------------------------------------------
 *  send process
 *-----------------------------------------------------------------------------*/
void send_proc (int p_id)
{
    int msq_id;
    st_msg_buf msg_buf;
    size_t buf_size;
    char input[MSGSIZE];

    msg_buf._lType = 1;
    msg_buf._iFrom_id = p_id;
 
    if ((msq_id = msgget(1234, 0666)) < 0)                  
    {                                                      
        perror("msgget");                                  
        exit(EXIT_FAILURE);                                
    }

    while (1)
    {
        fgets(input, MSGSIZE, stdin);
        bzero(msg_buf._cText, MSGSIZE);
        strncpy(msg_buf._cText, input, strlen(input) - 1);

        if (msgsnd(msq_id, &msg_buf, sizeof(st_msg_buf), IPC_NOWAIT) < 0)
        {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Message Sent: From[%d], Text[%s] \n", msg_buf._iFrom_id, msg_buf._cText);
        }
    }
}


/*-----------------------------------------------------------------------------
 *  receive process
 *-----------------------------------------------------------------------------*/
void recv_proc (int p_id)
{
    int msq_id;
    FILE *file_s;
    st_msg_buf msg_buf;

    if ((file_s = fopen("./2015-3-23.log", "a")) == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);                                
    }

    msg_buf._lType = 1;
    msg_buf._iTo_id = p_id;
    
    if ((msq_id = msgget(1234, 0666)) < 0)                  
    {                                                      
        perror("msgget");                                  
        exit(EXIT_FAILURE);                                
    }
 
    while (1)
    {
    /*  Receive message from queue */                   
        if (msgrcv(msq_id, &msg_buf, sizeof(st_msg_buf), 1, 0) < 0)   
        {                                                  
            perror("msgrcv");                              
            exit(EXIT_FAILURE);
        }
        else                                               
        {                                                  
            (void) fprintf(file_s, "Message Received: Text[%s] \n", msg_buf._cText);
        }                                                  
    }
    fclose(file_s);
}
/* -----  end of function recv_proc  ----- */

void init_handler (int signal)
{
    printf("signal: %d\n", signal);
    exit(0);
}
/* -----  end of function inti_handler  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Main
 *  Description:  Main function
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
    int msq_id;
    int proc_id;
    int proc_send_id, proc_recv_id;
    int msg_flg = IPC_CREAT | 0666;
    key_t key = 1234;

    if ((msq_id = msgget(key, msg_flg)) < 0) 
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    else 
    {
        (void) fprintf(stdout,"msgget: msgget succeeded: msq_id = %d\n", msq_id);
    }

    proc_id = fork();
    if (proc_id < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (proc_id == 0)
    {
        // Send engine
        printf("Parent id: %d, Child id: %d\n", getppid(), getpid());
        signal(SIGINT, init_handler);
        send_proc(getpid());
    }
    else
    {
        if (( proc_id = fork()) < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (proc_id == 0)
        {
            // Receive engine
            printf("Parent id: %d, Child id: %d\n", getppid(), getpid());
            signal(SIGINT, init_handler);
            recv_proc(getpid());
        }
        else
        {
            signal(SIGINT, SIG_IGN);
            wait();
        }
    }
    printf("Parent Process Exit!\n");

    return EXIT_SUCCESS;
}
/* ----------  end of function main  ---------- */

