#include "headers.h"
void restore_terminal_mode() 
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    flags &= ~O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, flags);
}
void set_noncanonical_mode()
{
    tcgetattr(STDIN_FILENO, &original_tio);
    struct termios newt = original_tio;

    // Set non-canonical mode and disable echo
    newt.c_lflag &= ~(ICANON | ECHO);

    // Apply the new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Make stdin non-blocking
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}
void neonate(char full_comm[], char dir_of_file[])
{
    char *comm_list[3];
    comm_list[0] = strtok(full_comm, " ");
    comm_list[1] = strtok(NULL, " ");
    comm_list[2] = strtok(NULL, " ");
    int n = atoi(comm_list[2]);
    set_noncanonical_mode();

    while (1)
    {
        DIR *dir;
        struct dirent *entry;
        int max_pid = -1;
        dir = opendir("/proc");
        if (dir == NULL)
        {
            perror("opendir");
            return;
        }
        while ((entry = readdir(dir)) != NULL)
        {
            int pid = atoi(entry->d_name);
            if (pid > 0)
            {
                if (pid > max_pid)
                {
                    max_pid = pid;
                }
            }
        }
        closedir(dir);
        if (max_pid != -1) 
        {
            printf("%d\n", max_pid);
        } else 
        {
            printf("Failed to get most recent process PID\n");
        }
        sleep(n);
        char ch = getchar();
        if (ch == 'x') 
        {
            break;
        }
    }
    restore_terminal_mode();
    return;
}