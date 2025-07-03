#include "headers.h"

void redirection(char full_comm[], char dir_of_file[], char *prev_rel_path, int *log_flag)
{
    char *comm_list[3];
    pid_t pid;
    if (strstr(full_comm, "<") != NULL && strstr(full_comm, ">>") != NULL)
    {
        // Parse command
        comm_list[0] = strtok(full_comm, "<"); // command to execute
        comm_list[1] = strtok(NULL, ">");      // input file
        int len = strlen(comm_list[1]);
        while (len > 0 && comm_list[1][len-1]==' ') 
        {
            len--;
        }
        comm_list[1][len] = '\0';
        comm_list[2] = strtok(NULL, ">");      // output file
        // Trim whitespace
        if (comm_list[1] != NULL)
        {
            while (*comm_list[1] == ' ')
                comm_list[1]++;
        }
        if (comm_list[2] != NULL)
        {
            while (*comm_list[2] == ' ' || *comm_list[2] == '>')
                comm_list[2]++;
        }
        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            return;
        }

        if (pid == 0)
        {
            // In child process
            int saved_stdout = dup(STDOUT_FILENO);
            int saved_stdin = dup(STDIN_FILENO);

            if (saved_stdout < 0 || saved_stdin < 0)
            {
                perror("dup error");
                return;
            }

            int file1 = open(comm_list[1], O_RDONLY);
            if (file1 < 0)
            {
                perror("open input file");
                return;
            }
            int file2 = open(comm_list[2], O_WRONLY | O_APPEND | O_CREAT, 0644);
            if (file2 < 0)
            {
                perror("open output file");
                close(file1); 
                return;
            }
            // Redirect stdin and stdout
            if (dup2(file1, STDIN_FILENO) < 0 || dup2(file2, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                close(file1);
                close(file2);
                return;
            }

            close(file1);
            close(file2);
            pipe_with_fgtime(comm_list[0], dir_of_file, prev_rel_path, log_flag);
            // Restore stdin and stdout
            if (dup2(saved_stdin, STDIN_FILENO) < 0 || dup2(saved_stdout, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                close(saved_stdin);
                close(saved_stdout);
                return;
            }
            close(saved_stdin);
            close(saved_stdout);
        }
        else
        {
            wait(NULL);
        }
    }
    else if (strstr(full_comm, ">") != NULL && strstr(full_comm, "<") != NULL)
    {
        // Parse command
        comm_list[0] = strtok(full_comm, "<"); // command to execute
        comm_list[1] = strtok(NULL, " ");      // input file
        comm_list[2] = strtok(NULL, ">");      // output file

        // Trim whitespace
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

        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            return;
        }

        if (pid == 0)
        {
            int saved_stdout = dup(STDOUT_FILENO);
            int saved_stdin = dup(STDIN_FILENO);

            if (saved_stdout < 0 || saved_stdin < 0)
            {
                perror("dup error");
                return;
            }

            int file1 = open(comm_list[1], O_RDONLY);
            if (file1 < 0)
            {
                perror("open input file");
                return;
            }
            int file2 = open(comm_list[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (file2 < 0)
            {
                perror("open output file");
                close(file1); 
                return;
            }

            // Redirect stdin and stdout
            if (dup2(file1, STDIN_FILENO) < 0 || dup2(file2, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                close(file1);
                close(file2);
                return;
            }

            close(file1);
            close(file2);
            pipe_with_fgtime(comm_list[0], dir_of_file, prev_rel_path, log_flag);

            if (dup2(saved_stdin, STDIN_FILENO) < 0 || dup2(saved_stdout, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                close(saved_stdin);
                close(saved_stdout);
                return;
            }
            close(saved_stdin);
            close(saved_stdout);
        }
        else
        {
            wait(NULL);
        }
    }
    else if (strstr(full_comm, ">>") != NULL)
    {
        // Parse command
        comm_list[0] = strtok(full_comm, ">");
        comm_list[1] = strtok(NULL, ">");
        // printf ("%s\n", comm_list[0]);
        // printf ("%s\n", comm_list[1]);
        if (comm_list[1] != NULL)
        {
            while (*comm_list[1] == ' ')
                comm_list[1]++;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            return;
        }

        if (pid == 0)
        {
            // In child process
            int saved_stdout = dup(STDOUT_FILENO);
            if (saved_stdout < 0)
            {
                perror("dup error");
                return;
            }

            int file = open(comm_list[1], O_WRONLY | O_APPEND | O_CREAT, 0644);
            if (file < 0)
            {
                perror("open output file");
                return;
            }

            if (dup2(file, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                close(file);
                return;
            }

            close(file);
            pipe_with_fgtime(comm_list[0], dir_of_file, prev_rel_path, log_flag);

            if (dup2(saved_stdout, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                close(saved_stdout);
                return;
            }
            close(saved_stdout);
        }
        else
        {
            wait(NULL);
        }
    }
    else if (strstr(full_comm, ">") != NULL)
    {
        // Parse command
        comm_list[0] = strtok(full_comm, ">");
        comm_list[1] = strtok(NULL, " ");

        if (comm_list[1] != NULL)
        {
            while (*comm_list[1] == ' ')
                comm_list[1]++;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            return;
        }

        if (pid == 0)
        {
            // In child process
            int saved_stdout = dup(STDOUT_FILENO);
            if (saved_stdout < 0)
            {
                perror("dup error");
                return;
            }

            int file = open(comm_list[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (file < 0)
            {
                perror("open output file");
                return;
            }

            if (dup2(file, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                close(file);
                return;
            }

            close(file);
            pipe_with_fgtime(comm_list[0], dir_of_file, prev_rel_path, log_flag);
            if (dup2(saved_stdout, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                close(saved_stdout);
                return;
            }
            close(saved_stdout);
        }
        else
        {
            wait(NULL);
        }
    }
    else if (strstr(full_comm, "<") != NULL)
    {
        // Parse command
        comm_list[0] = strtok(full_comm, "<");
        comm_list[1] = strtok(NULL, " ");

        if (comm_list[1] != NULL)
        {
            while (*comm_list[1] == ' ')
                comm_list[1]++;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            return;
        }

        if (pid == 0)
        {
            // In child process
            int saved_stdin = dup(STDIN_FILENO);
            if (saved_stdin < 0)
            {
                perror("dup error");
                return;
            }

            int file = open(comm_list[1], O_RDONLY);
            if (file < 0)
            {
                perror("open input file");
                return;
            }

            if (dup2(file, STDIN_FILENO) < 0)
            {
                perror("dup2");
                close(file);
                return;
            }

            close(file);
            pipe_with_fgtime(comm_list[0], dir_of_file, prev_rel_path, log_flag);

            if (dup2(saved_stdin, STDIN_FILENO) < 0)
            {
                perror("dup2");
                close(saved_stdin);
                return;
            }
            close(saved_stdin);
        }
        else
        {
            wait(NULL);
        }
    }
}
