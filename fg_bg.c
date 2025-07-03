#include "headers.h"
void fg (char full_comm[], char dir_of_file[], int pid)
{
    for (int i=0; i<num_act_list; i++)
    {
        if (act_list[i]->pid==pid)
        {
            int status;
            if (strstr (act_list[i]->state, "Stopped")!=NULL)
            {
                int sig = 18; // sig for cont
                if (kill(pid, sig)!=0) 
                {
                    perror("No such process found");
                    return;
                } 
                waitpid (pid, NULL, 0);
                strcpy (act_list[i]->state, "Terminated");
            }
            else
            {
                if (waitpid(pid, NULL, 0) < 0) 
                {
                    perror("Error waiting for process");
                    return;
                }
                strcpy (act_list[i]->state, "Terminated");
            }      
            return;     
        }
        else
        {
            perror("No such process found");
            return;
        }
    }
    
}
void bg (char full_comm[], char dir_of_file[], int pid)
{  
    int sig = 18; // sig for cont  
    signal(SIGTERM, sigchld_handler);
    if (kill(pid, sig)!=0) 
    {
        perror("No such process found");
        return;
    } 
    return;
}
void fg_bg (char full_comm[], char dir_of_file[])
{
    char* comm_list[2];
    comm_list[0]= strtok (full_comm, " ");
    comm_list[1]= strtok (NULL, " ");
    if (comm_list[0] != NULL)
    {
        while (*comm_list[0] == ' ')
            comm_list[0]++;
    }
    if (comm_list[1] != NULL)
    {
        while (*comm_list[1] == ' ')
            comm_list[1]++;
    }
    int pid= atoi (comm_list[1]);
    if (strstr (comm_list[0], "fg")!=NULL)
    {
        fg (full_comm, dir_of_file, pid);
    }
    else
    {
        bg (full_comm, dir_of_file, pid);
    }
    return;
}