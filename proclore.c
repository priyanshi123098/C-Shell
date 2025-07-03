#include "headers.h"
void proclore(char full_comm[], char dir_of_file[])
{
    char delim[] = " ";
    char *token = strtok(full_comm, delim);
    token = strtok(NULL, delim);
    int pid;
    // print info of shell if argumemt is missing
    if (token == NULL)
    {
        pid = getpid();
    }
    // pid is given
    else
    {
        if (token != NULL) {
            while (*token == ' ' || *token == '\t') token++;
        }
        for (int i = 0; i < strlen(token); i++)
        {
            pid = pid + ((token[i] - 48) * pow(10, strlen(token) - 1 - i));
        }
    }
    char path[4096];
    char buffer[4096];
    FILE *fp;
    char status[16];
    long int pid_group;
    unsigned long virtual_memory;

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    fp = fopen(path, "r");
    if (fp == NULL)
    {
        perror("Error opening stat file, process doen't exist");
        return;
    }
    long unsigned temp;
    char temp_str[4096];
    fscanf(fp, "%lu", &temp);  
    fscanf(fp, "%s", temp_str); 

    // Read the status (3rd field)
    fscanf(fp, " %s", status);
    for (int i = 0; i < 4; i++)
    {
        fscanf(fp, "%ld", &temp);
    }

    fscanf(fp, "%ld", &pid_group);
    for (int i = 0; i < 16; i++)
    {
        fscanf(fp, "%lu", &temp);
    }
    fscanf(fp, "%lu", &virtual_memory);
    pid_t process_pgid = getpgid(pid);
    if (process_pgid == -1) {
        perror("Failed to get process group ID");
        return;
    }
    pid_t fg_pgid = tcgetpgrp(STDIN_FILENO);
    if (fg_pgid == -1) {
        perror("Failed to get foreground process group ID");
        return;
    }
    printf("pid : %d\n", pid);
    
    if (pid== getpid())     // foreground process
    printf("process status : %s+\n", status);
    else
    printf("process status : %s\n", status);

    printf("process group : %ld\n", pid_group);
    printf("virtual memory : %lu\n", virtual_memory);

    snprintf(path, sizeof(path), "/proc/%d/exe", pid);
    ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);
    if (len != -1)
    {
        buffer[len] = '\0'; // Null-terminate the path
    }
    else
    {
        strcpy(buffer, "Executable doesn't exist");
    }
    char rel_path[4096];
    if (strstr(buffer, dir_of_file) != NULL) 
    {
        snprintf(rel_path, sizeof(rel_path), "~%s", buffer + strlen(dir_of_file));
        printf("executable Path : %s\n", rel_path);
    } 
    else 
    {
        snprintf(rel_path, sizeof(rel_path), "%s", buffer);
        printf("executable Path : %s\n", buffer);
    }
}
