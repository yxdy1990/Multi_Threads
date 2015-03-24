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



FILE *file_s = NULL;

/*-----------------------------------------------------------------------------
 *  send process
 *-----------------------------------------------------------------------------*/
void send_proc (int p_id)
{
    int msq_id;
    st_msg_buf msg_buf;
    size_t buf_size;
    int index;
    char input[3][MSGSIZE] = {"Hello world!", "I love you!", "Very good!"};

    msg_buf._lType = 1;
    msg_buf._iFrom_id = p_id;
   
    if ((msq_id = msgget(1234, 0666)) < 0)                  
    {                                                      
        perror("msgget");                                  
        exit(EXIT_FAILURE);                                
    }

   // while (1)
    //{
        //fgets(input, MSGSIZE, stdin);
        for (index = 0; index < 3; index++)
        {
        bzero(msg_buf._cText, MSGSIZE);
        strncpy(msg_buf._cText, input[index], strlen(input[index]) - 1);

        if (msgsnd(msq_id, &msg_buf, sizeof(msg_buf._cText), IPC_NOWAIT) < 0)
        {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Message Sent: From[%d], Text[%s] \n", msg_buf._iFrom_id, msg_buf._cText);
        }
        sleep(3);
        }
    //}
}


/*-----------------------------------------------------------------------------
 *  receive process
 *-----------------------------------------------------------------------------*/
void recv_proc (int p_id)
{
    int msq_id;
    st_msg_buf msg_buf;

    if ((file_s = fopen("./2015-3-23.log", "a")) == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);                                
    }
    (void) fprintf(file_s, "\nNew start:\n");
    (void) fprintf(file_s, "Open log file success!\n");

    msg_buf._lType = 1;
    msg_buf._iTo_id = p_id;
    
    if ((msq_id = msgget(1234, 0666)) < 0)                  
    {                                                      
        perror("msgget");                                  
        exit(EXIT_FAILURE);                                
    }
    else 
    {
        (void) fprintf(file_s,"Recv[msgget]: msgget succeeded: msq_id = %d\n", msq_id);
    }
    while (1)
    {
        (void) fprintf(file_s, "Enter Recv loop!\n");
    /*  Receive message from queue */                   
        if (msgrcv(msq_id, &msg_buf, MSGSIZE, 1, 0) < 0)   
        {                                                  
            perror("msgrcv");                              
            (void) fprintf(file_s, "Msgrecv Error\n");
            exit(EXIT_FAILURE);
        }
        else                                               
        {                                                  
            (void) fprintf(file_s, "Message Received: Text[%s] \n", msg_buf._cText);
        }                                                  
    }
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
    struct msqid_ds msq_set;
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
    
/*  if (msgctl(msq_id, IPC_STAT, &msq_set) < 0)
    {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }
    msq_set.msg_qbytes = 65536;
    if (msgctl(msq_id, IPC_SET, &msq_set) < 0)
    {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }
*/
    if ((proc_id = fork()) < 0)
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
        }
        wait(NULL);   
        
        if (file_s != NULL)
        {
            fclose(file_s);
        }
        
        msgctl(msq_id, IPC_RMID, NULL); // Remove message queue
        
        printf("Parent Process Exit!\n");
       
        return EXIT_SUCCESS;
    }
}
/* ----------  end of function main  ---------- */

