#include "headers.h"
void sigtstp_handler(int sig) {
    if (last_fg_pid > 0) {
            signal (SIGTSTP,  sigchld_handler);

        if (kill(last_fg_pid, SIGTSTP) ==0)

       {
                printf("\nForeground process %d stopped, moving to background...\n", last_fg_pid);

                act_list[num_act_list] = (info *)malloc(sizeof(struct info));
                if (act_list[num_act_list] == NULL)
                {
                    perror("malloc failed");
                    return;
                }
                info *temp2 = (info *)malloc(sizeof(info));
                strcpy (temp2->command, last_fg_name);
                temp2->pid = last_fg_pid;
                strcpy(temp2->state, "Running");
                act_list[num_act_list] = temp2;
                num_act_list++;
        // signal(SIGTSTP, sigchld_handler);

        // printf ("#########\n");
        // printf ("%d\n", last_fg_pid);
        // kill(last_fg_pid, SIGCONT); 
        // printf ("@@@@@@@@@@@\n"); 
        // printf ("%d\n", last_fg_pid);
        // kill (last_fg_pid, SIGTSTP);
                last_fg_pid=-1;
                // printf ("@@@@");
                fflush (stdout);
                // printf ("1234");
       }
        // Send SIGTSTP to the foreground process
        // Now move it to the background
    }
    // printf ("^^^^^^^^^^^^");
    return;
}
void handle_sigkill()
{
    for (int i=0; i<num_act_list; i++)
    {
        if (strstr (act_list[i]->state, "Stopped")!=NULL || strstr (act_list[i]->state, "Running")!=NULL)
        {
            kill (act_list[i]->pid, SIGKILL);
        }
    }
    exit (0);
}
void handle_sigint(int sig) 
{
    // printf ("%d#######\n", last_fg_pid);
    if (last_fg_pid!=-1)
    {
        kill (last_fg_pid, SIGINT);
        last_fg_pid=-1;
        return;
    }
    else
    {
        return;
    }
}
void signals (char full_comm[], char dir_of_file[])
{
    char* comm_list[3];
    comm_list[0] = strtok(full_comm, " "); // command to execute
    comm_list[1] = strtok(NULL, " ");      // input file
    comm_list[2] = strtok(NULL, " ");
    if (comm_list[1] != NULL)
    {
        while (*comm_list[1] == ' ')
            comm_list[1]++;
    }
    if (comm_list[2] != NULL)
    {
        while (*comm_list[2] == ' ')
            comm_list[2]++;
    }
    int pid = atoi(comm_list[1]);  
    int sig = atoi(comm_list[2]);  
    int x= sig;
    sig= (sig%32);
    signal(SIGTERM, sigchld_handler);
    if (kill(pid, sig)!=0) 
    {
        perror("Failed to send signal");
        return;
    } 
    printf("Sent signal %d to process with pid %d\n", x, pid);
    return;
}