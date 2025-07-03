#include "headers.h"
void pipes(char full_comm[], char dir_of_file[], char *prev_rel_path, int *log_flag) {
    char delim[] = "|";
    char* cmd_list[100];
    int k=0;
    int pipe_flag=0;
    int leng = strlen(full_comm);

    if (strstr (full_comm, "||")!=NULL)
    {
        printf ("Invalid: consecutive pipes\n");
        return;
    }
    while (leng > 0 && full_comm[leng-1]==' ') 
    {
        leng--;
    }
    full_comm[leng] = '\0';
    if (full_comm[leng-1]=='|')
    {
        printf ("Invalid: Pipe at last\n");
        return;
    }
    // Tokenize based on pipes
    cmd_list[k] = strtok(full_comm, delim);
    while (cmd_list[k] != NULL) 
    {
        k++;
        cmd_list[k] = strtok(NULL, delim);
    }

    int pipefd[2];  // Pipe file descriptors
    int in_fd = 0;  
    for (int i = 0; i < k; i++) 
    {
        int len = strlen(cmd_list[i]);
        while (len > 0 && cmd_list[i][len-1]==' ') 
        {
            len--;
        }
        if (len==0)
        {
            printf ("Invalid: consecutive pipes\n");
            return;
        }
        cmd_list[i][len] = '\0';
        if (cmd_list[i] != NULL) 
        {
            while (*cmd_list[i] == ' ' || *cmd_list[i] == '\t') cmd_list[i]++;
        }

        // Create a pipe for each command except the last one
        if (i < k - 1) {
            if (pipe(pipefd) == -1) {
                perror("pipe failed");
                return;
            }
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            return;
        }

        if (pid == 0) {
            if (in_fd != 0) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }

            if (i < k - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            if (strchr(cmd_list[i], '>') != NULL || strchr(cmd_list[i], '<') != NULL) {
                redirection(cmd_list[i], dir_of_file, prev_rel_path, log_flag);
            } else {
                pipe_with_fgtime(cmd_list[i], dir_of_file, prev_rel_path, log_flag);
            }

            exit(EXIT_SUCCESS);  
        } 
        else 
        {
            wait(NULL);
            if (in_fd != 0) {
                close(in_fd);  // Close the previous read end
            }
            if (i < k - 1) {
                close(pipefd[1]);  // Close the current write end
            }

            in_fd = pipefd[0];  
        }
    }
}
