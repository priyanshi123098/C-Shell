#include "headers.h"

info ** act_list= NULL;
int num_act_list=0;
pid_t last_fg_pid=-1;
struct termios original_tio;
char* last_fg_name;
int main()
{
    act_list= (info**)malloc (sizeof (info*)*100);
//    home dir where the code is written
    char dir_of_file[4096];
    if (getcwd (dir_of_file, sizeof (dir_of_file))==NULL)
    {
        perror("cwd path not obtained");
        return -1;
    }
    
    char system_name[4096];
    int temp= gethostname (system_name, sizeof (system_name));
    if (temp != 0) 
    {
        perror("error in system name");
        return -1;
    }
    char * username= getlogin();
    if (username == NULL) 
    {
        perror("error in username");
        return -1;
    }

    // ############# FOR LOG FUNCTIONALITY #########################
    FILE *filePtr;
    filePtr = fopen("log_file.txt", "a");
    if (filePtr == NULL) {
        perror ("log file not formed");
        return -1;
    }
    fclose(filePtr);
    
    char* rel_path;
    char* prev_rel_path= (char*)malloc (sizeof (char) *4096);
    char *curr_dir_path= (char*)malloc (sizeof (char) *4096);
    int t=0;
    // signal(SIGCHLD, sigchld_handler);
    signal(SIGINT, handle_sigint);
    while (1)
    {
        strcpy (prev_rel_path, curr_dir_path);
        if (getcwd (curr_dir_path, 4096)==NULL)
        {
            perror("cwd path not obtained");
            return -1;
        }
        if (strstr(curr_dir_path, dir_of_file) != NULL) 
        {
            rel_path= curr_dir_path+ strlen (dir_of_file);
            if (*rel_path == '/') 
            {
                rel_path++;
            }
            if (t<=2)
            printf("\033[33m<%s@%s:~%s>\033[0m ", username, system_name, rel_path);
            else
            {
                printf("\033[33m<%s@%s:~%s sleep : %d>\033[0m ", username, system_name, rel_path, t);
            }
        } 
        else 
        {
            strcpy (rel_path, curr_dir_path);
            if (t<=2)
            printf("\033[33m<%s@%s:%s>\033[0m  ", username, system_name, rel_path);
            else
            {
                printf("\033[33m<%s@%s:%s sleep : %d>\033[0m ", username, system_name, rel_path, t);
            }
        }
        t=0;
        char full_comm[4096];
        // scanf (" %[^\n]", full_comm);
            signal(SIGTSTP, sigtstp_handler);

        if (fgets (full_comm, sizeof (full_comm), stdin)!=NULL)
        {
            size_t len = strlen(full_comm);
            while (len > 0 && (full_comm[len-1] == '\n' || full_comm[len-1]==' ' || full_comm[len-1]=='\t')) 
            {
                len--;
            }
            full_comm[len] = '\0';
            t= split_input (full_comm, dir_of_file, prev_rel_path);
        }
        else
        {
            handle_sigkill();
        }
    }
    return 0;
}