#include "headers.h"
// here may include io redirection
void sigchld_handler(int sig)
{
    int status;
    pid_t p;
    while ((p = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0)
    {
        // printf ("%d", p);
        for (int i = 0; i < num_act_list; i++)
        {
            if (act_list[i]->pid == p)
            {
                if (WIFEXITED(status))
                {
                    printf("normal termination  (%d)\n", p);
                    for (int i = 0; i < num_act_list; i++)
                    {
                        if (act_list[i]->pid == p)
                        {
                            strcpy(act_list[i]->state, "Terminated");
                        }
                    }
                    return;
                }
                else if (WIFSIGNALED(status))
                {
                    // Child terminated by a signal
                    // printf("Process %d was killed by signal %d\n", p, WTERMSIG(status));
                    for (int i = 0; i < num_act_list; i++)
                    {
                        if (act_list[i]->pid == p)
                        {
                            strcpy(act_list[i]->state, "Killed");
                        }
                    }
                    return;
                }
                else if (WIFSTOPPED(status))
                {
                    // printf ("#########");
                    // Child was stopped by a signal (e.g., SIGSTOP)
                    // printf("Process %d was stopped by signal %d\n", p, WSTOPSIG(status));
                    for (int i = 0; i < num_act_list; i++)
                    {
                        if (act_list[i]->pid == p)
                        {
                            strcpy(act_list[i]->state, "Stopped");
                        }
                    }
                    return;
                }
                else if (WIFCONTINUED(status))
                {
                    // Child continued after being stopped (e.g., SIGCONT)
                    // printf("Process %d was resumed\n", p);
                    // printf ("aaaaaaaa");
                    for (int i = 0; i < num_act_list; i++)
                    {
                        if (act_list[i]->pid == p)
                        {
                            strcpy(act_list[i]->state, "Running");
                        }
                    }
                    return;
                    // }
                }
            }
        }
    }
    return;
}
void execute_command_background(char *input, char dir_of_file[])
{
    char *args[100]; 
    char temp_input[4096];
    strcpy(temp_input, input);
    char *token = strtok(input, " ");
    int i = 0;
    signal(SIGCHLD, sigchld_handler); 
    while (token != NULL)
    {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    signal(SIGCHLD, sigchld_handler); // Set up signal handling
    int pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        return;
    }
    else if (pid>0)
    {
        printf("Background process PID: %d\n", pid);
        // Add to act_list
        act_list[num_act_list] = (info *)malloc(sizeof(struct info));
        if (act_list[num_act_list] == NULL)
        {
            perror("malloc failed");
            return;
        }

        info *temp2 = (info *)malloc(sizeof(info));
        strcpy(temp2->command, temp_input);
        temp2->pid = pid;
        strcpy(temp2->state, "Running");
        act_list[num_act_list] = temp2;
        num_act_list++;
        return;
    }
    else 
    {
        if (execvp(args[0], args) == -1)
        {
            perror("execvp failed");
            return;
        }
    }
    
    // return;
}

int execute_command_foreground(char *input, char dir_of_file[])
{
    char *args[100]; 
    char *token = strtok(input, " ");
    int i = 0;
    while (token != NULL)
    {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    struct timeval start, end;
    double elapsed_time;

    gettimeofday(&start, NULL); 
    int pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        return -1;
    }
    else if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            printf("Invalid Command\n");
            return 0;
        }
    }
    else
    {
        waitpid (pid, NULL, 0);
        // tcsetpgrp (0, pid);
    }
    gettimeofday(&end, NULL);                                                               // Record end time
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0; // Calculate elapsed time

    return ((int)(elapsed_time));
}
// void execute_command_background(char *input, char dir_of_file[])
// {
//     char *args[100]; // Command arguments
//     char temp_input[4096];
//     strcpy(temp_input, input);
//     char *token = strtok(input, " ");
//     int i = 0;
//     signal(SIGCHLD, sigchld_handler); // Ensure signal handling is set for background processes
//     while (token != NULL)
//     {
//         args[i] = token;
//         i++;
//         token = strtok(NULL, " ");
//     }
//     args[i] = NULL;
//     // signal(SIGCHLD, sigchld_handler); // Set up signal handling
//     int pid = fork();
//     if (pid < 0)
//     {
//         perror("Fork failed");
//         return;
//     }
//     else if (pid>0)
//     {
//         printf("Background process PID: %d\n", pid);
//         // Add to act_list (your implementation here)
//         act_list[num_act_list] = (info *)malloc(sizeof(struct info));
//         if (act_list[num_act_list] == NULL)
//         {
//             perror("malloc failed");
//             return;
//         }

//         info *temp2 = (info *)malloc(sizeof(info));
//         strcpy(temp2->command, temp_input);
//         temp2->pid = pid;
//         strcpy(temp2->state, "Running");
//         act_list[num_act_list] = temp2;
//         num_act_list++;
//         return;
//     }
//     else 
//     {
//         // Child process
//         if (execvp(args[0], args) == -1)
//         {
//             perror("execvp failed");
//             return; // Exit child process if execvp fails
//         }
//     }
    
//     // return;
// }

// int execute_command_foreground(char *input, char dir_of_file[])
// {
//     char *args[100]; // Command arguments
//     char *token = strtok(input, " ");
//     int i = 0;
//     while (token != NULL)
//     {
//         args[i] = token;
//         i++;
//         token = strtok(NULL, " ");
//     }
//     args[i] = NULL;
//     struct timeval start, end;
//     double elapsed_time;

//     gettimeofday(&start, NULL); // Record start time
//     int pid = fork();
//     if (pid < 0)
//     {
//         perror("Fork failed");
//         return -1;
//     }
//     else if (pid == 0)
//     {
//         if (execvp(args[0], args) == -1)
//         {
//             printf("Invalid Command\n");
//             return 0;
//         }
//     }
//     else
//     {
//         waitpid (pid, NULL, 0);
//     }
//     gettimeofday(&end, NULL);                                                               // Record end time
//     elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0; // Calculate elapsed time

//     return ((int)(elapsed_time));
// }
int sys_comm(char full_comm[], char dir_of_file[])
{
    char *commands[100];
    int i = 0;
    // Tokenize the input line by "&" to get individual commands
    if (strstr(full_comm, "&") == NULL)
    {
        char *token = strtok(full_comm, "&");
        int t = execute_command_foreground(token, dir_of_file);
        return t;
    }
    else
    {
        int t = 0;
        char command1[4096];
        char command2[4096];
        char *token = strtok(full_comm, "&");
        if (token != NULL)
        {
            strcpy(command1, token);
            execute_command_background(command1, dir_of_file);
        }
        token = strtok(NULL, "&");
        if (token != NULL)
        {
            strcpy(command2, token);
            t = execute_command_foreground(command2, dir_of_file);
        }
        else
            return 0;

        return t;

        // for (int i=0; i<2; i++)
        // {
        //     if (i==0)
        //     {
        //         if (token != NULL)
        //         {
        //             strcpy(command1, token);
        //             execute_command_background(command1, dir_of_file);
        //             token = strtok(NULL, "&");
        //         }
        //     }
        //     else
        //     {
        //         if (token != NULL)
        //         {
        //             strcpy(command2, token);
        //             t = execute_command_foreground(command2, dir_of_file);
        //             token = strtok(NULL, "&");
        //         }
        //     }
        // }
        
    }
}

