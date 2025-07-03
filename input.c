#include "headers.h"
int pipe_with_fgtime(char full_comm[], char dir_of_file[], char *prev_rel_path, int *log_flag)
{
    int len = strlen(full_comm);
    while (len > 0 && (full_comm[len-1]==' ' || full_comm[len-1]=='\t') || full_comm[len-1]=='\n') 
    {
        len--;
    }
    full_comm[len] = '\0';
    if (full_comm != NULL)
    {
        while (*full_comm == ' '  || *full_comm=='\t')
            full_comm++;
    }
    last_fg_pid= getpid();
    last_fg_name= full_comm;
    int time_fg = 0;
    char store_comm_string[4096];
    strcpy(store_comm_string, full_comm);
    char delim2[] = " ";
    char *token2 = strtok(store_comm_string, delim2);
    if (strcmp(token2, "hop") == 0)
    {
        char curr_dir_path[4096];
        if (getcwd(curr_dir_path, 4096) == NULL)
        {
            perror("cwd path not obtained");
            return -1;
        }
        hop(dir_of_file, full_comm, prev_rel_path);
    }
    else if (strcmp(token2, "reveal") == 0)
    {
        reveal(full_comm, dir_of_file, prev_rel_path);
    }
    else if (strcmp(token2, "log") == 0)
    {
        *log_flag=1;
        if (strcmp(full_comm, "log") == 0)
        {
            log_show(dir_of_file);
        }
        else if (strstr (full_comm, "purge")!=NULL)
        {
            log_purge(dir_of_file);
        }
        else
        {
            log_execute(full_comm, dir_of_file, prev_rel_path);
        }
    }
    else if (strcmp(token2, "proclore") == 0)
    {
        proclore(full_comm, dir_of_file);
    }
    else if (strcmp(token2, "seek") == 0)
    {
        seek(full_comm, dir_of_file, prev_rel_path);
    }
    else if (strcmp(token2, "activities") == 0)
    {
        activities();
    }
    else if (strcmp(token2, "ping") == 0)
    {
        signals (full_comm, dir_of_file);
    }
    else if (strcmp(token2, "iMan") == 0)
    {
        iman (full_comm, dir_of_file);
    }
    else if (strcmp(token2, "fg") == 0 || strcmp(token2, "bg") == 0)
    {
        fg_bg (full_comm, dir_of_file);
    }
    else if (strcmp(token2, "neonate") == 0)
    {
        neonate(full_comm, dir_of_file);
    }
    else if (strcmp (token2, "exit")==0)
    {
        exit (0);
    }
    // bg fg
    else
    {
        time_fg += execute_command_foreground(full_comm, dir_of_file);
    }
    return time_fg;
}
int split_input(char full_comm[], char dir_of_file[], char *prev_rel_path)
{
    // splitting on basis of semi-colon
    char store_comm_for_log[4096];
    strcpy(store_comm_for_log, full_comm);

    char delim[] = ";";
    char *token = strtok(full_comm, delim);
    int log_flag = 0;
    char **comm_string = (char **)malloc(sizeof(char *) * 10);
    for (int i = 0; i < 10; i++)
    {
        comm_string[i] = (char *)malloc(4076 * sizeof(char));
        if (comm_string[i] == NULL)
        {
            fprintf(stderr, "Memory allocation failed for string\n");
            return -1;
        }
    }
    int c = 0;
    while (token != NULL)
    {
        strcpy(comm_string[c], token);
        if (comm_string[c] != NULL)
        {
            while (*comm_string[c] == ' ')
                comm_string[c]++;
        }
        c++;
        token = strtok(NULL, delim);
    }
    int time_fg = 0;
    for (int i = 0; i < c; i++)
    {
        int len = strlen(comm_string[i]);
        while (len > 0 && (comm_string[i][len-1]==' ' || comm_string[i][len-1]=='\t') || comm_string[i][len-1]=='\n') 
        {
            len--;
        }
        comm_string[i][len] = '\0';
        if (comm_string[i] != NULL)
        {
            while (*comm_string[i] == ' '  || *comm_string[i]=='\t')
                comm_string[i]++;
        }
        // only consists of 
        if (strstr(comm_string[i], "|") == NULL && strstr(comm_string[i], "&") == NULL 
        && strstr(comm_string[i], ">") == NULL && strstr(comm_string[i], "<") == NULL)
            time_fg += pipe_with_fgtime(comm_string[i], dir_of_file, prev_rel_path, &log_flag);
        else if (strstr(comm_string[i], "&") !=NULL)
            sys_comm(comm_string[i], dir_of_file);
        else
            pipes (comm_string[i], dir_of_file, prev_rel_path, &log_flag );
    }
    if (log_flag == 0)
    {
        log_append(store_comm_for_log, dir_of_file);
    }
    return time_fg;
}
