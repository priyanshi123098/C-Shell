#include "headers.h"
int compare(const void *a, const void *b) 
{
    struct dirent **entryA = (struct dirent **)a;
    struct dirent **entryB = (struct dirent **)b;
    return strcmp((*entryA)->d_name, (*entryB)->d_name);
}
void reveal(char full_comm[], char dir_of_file[], char* prev_rel_path) 
{
    int c = 1; // represents total num of strings in full_comm
    int flag = -1;
    char delim[] = " ";
    char *token = strtok(full_comm, delim);
    char *name_of_dir = (char *)malloc(4096 * sizeof(char));
    token = strtok(NULL, delim);
    if (token != NULL)
        strcpy(name_of_dir, token);

    while (token != NULL) 
    {
        if (name_of_dir[0] == '-') 
        {
            if (strlen(name_of_dir) != 1)    // to tackle the case when only - is there
            {
                char *temp1 = strstr(name_of_dir, "a");
                char *temp2 = strstr(name_of_dir, "l");
                if (temp1 != NULL && temp2 != NULL) 
                {
                    flag = 2;
                } 
                else if (temp1 == NULL)         // a not present
                { 
                    if (flag == -1 || flag == 1)
                        flag = 1;
                    else
                        flag = 2;
                } 
                else if (temp2 == NULL)         // l not present
                { 
                    if (flag == -1 || flag == 0)
                        flag = 0;
                    else
                        flag = 2;
                }
            }
        }
        strcpy(name_of_dir, token);
        c++;
        token = strtok(NULL, delim);
    }

    char curr_dir_path[4096];
    if (name_of_dir[0] != '/') 
    {
        if (c != 1)         // only reveal is not there
        {
            if (strcmp (name_of_dir, "~")==0)
            {
                strcpy(name_of_dir, dir_of_file);
            }
            else if (strcmp (name_of_dir, "-")==0)
            {
                strcpy(name_of_dir, prev_rel_path);
            }
            else if (name_of_dir[0]!='-')
            {
                if (getcwd(curr_dir_path, 4096) == NULL) 
                {
                    perror("cwd path not obtained");
                    return;
                }
                char temp[4096] = "/";
                strcat(temp, name_of_dir);
                strcpy(name_of_dir, curr_dir_path);
                strcat(name_of_dir, temp);
            }
            else
            {
                if (getcwd(curr_dir_path, 4096) == NULL) 
                {
                    perror("cwd path not obtained");
                    return;
                }
                strcpy(name_of_dir, curr_dir_path);
            }
        } 
        else if (c == 1) 
        {
            if (getcwd(curr_dir_path, 4096) == NULL) 
            {
                perror("cwd path not obtained");
                return;
            }
            strcpy(name_of_dir, curr_dir_path);
        }
    }

    struct dirent *entry;
    struct dirent **entry_list = (struct dirent **)malloc(100 * sizeof(struct dirent *));
    struct stat statbuf;
    DIR *dir = opendir(name_of_dir);
    char fullpath[4096];
    if (dir == NULL) 
    {
        perror("opendir");
        return;
    }

    int k = 0;
    while ((entry = readdir(dir)) != NULL) 
    {
        // full path of the dir/ file
        snprintf(fullpath, sizeof(fullpath), "%s/%s", name_of_dir, entry->d_name);
        if (stat(fullpath, &statbuf) == 0) 
        {
            entry_list[k] = (struct dirent*)malloc(sizeof(struct dirent));
            if (entry_list[k] == NULL)
            {
                perror("malloc");
                return;
            }
            memcpy(entry_list[k], entry, sizeof(struct dirent));
            k++;
        } 
        else 
        {
            perror("stat");
        }
    }

    int n = k;
    qsort(entry_list, n, sizeof(struct dirent *), compare);

    if (flag==-1)
    {
        for (int i = 0; i < n; i++) 
        {
            if (entry_list[i]->d_name[0] == '.') 
            {
                continue;
            }
            snprintf(fullpath, sizeof(fullpath), "%s/%s", name_of_dir, entry_list[i]->d_name);
            if (stat(fullpath, &statbuf) == 0) 
            {
                // directory in blue
                if (S_ISDIR(statbuf.st_mode)) 
                {
                    printf("\033[1;34m%s\033[0m\n", entry_list[i]->d_name);
                }
                // executable in green
                else if (S_ISREG(statbuf.st_mode) && (statbuf.st_mode & S_IXUSR)) 
                {
                    printf("\033[1;32m%s\033[0m\n", entry_list[i]->d_name);
                }
                // files in white
                else if (S_ISREG(statbuf.st_mode)) 
                {
                    printf("\033[1;37m%s\033[0m\n", entry_list[i]->d_name);
                }
            }
        }
    }
    else if (flag==0)
    {
        for (int i = 0; i < n; i++) 
        {
            snprintf(fullpath, sizeof(fullpath), "%s/%s", name_of_dir, entry_list[i]->d_name);
            if (stat(fullpath, &statbuf) == 0) 
            {
                // directory in blue
                if (S_ISDIR(statbuf.st_mode)) 
                {
                    printf("\033[1;34m%s\033[0m\n", entry_list[i]->d_name);
                }
                // executable in green
                else if (S_ISREG(statbuf.st_mode) && (statbuf.st_mode & S_IXUSR)) 
                {
                    printf("\033[1;32m%s\033[0m\n", entry_list[i]->d_name);
                }
                // files in white
                else if (S_ISREG(statbuf.st_mode)) 
                {
                    printf("\033[1;37m%s\033[0m\n", entry_list[i]->d_name);
                }
            }
        }
    }
    else if (flag==1)
    {
        for (int i = 0; i < n; i++) 
        {
            if (entry_list[i]->d_name[0] == '.' && flag == 1) 
            {
                continue;
            }
            snprintf(fullpath, sizeof(fullpath), "%s/%s", name_of_dir, entry_list[i]->d_name);
            if (stat(fullpath, &statbuf) == 0) 
            {
                printf((S_ISDIR(statbuf.st_mode)) ? "d" :
                       (S_ISLNK(statbuf.st_mode)) ? "l" : "-");
                printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
                printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
                printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
                printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
                printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
                printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
                printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
                printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
                printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");

                printf(" %ld", (long) statbuf.st_nlink);

                // Print owner and group names
                struct passwd *pw = getpwuid(statbuf.st_uid);
                struct group *gr = getgrgid(statbuf.st_gid);
                printf(" %s %s", pw->pw_name, gr->gr_name);

                // Print file size
                printf(" %ld", (long) statbuf.st_size);

                // Print last modification time
                char timebuf[80];
                struct tm lt;
                localtime_r(&statbuf.st_mtime, &lt);
                strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", &lt);
                printf(" %s", timebuf);

                // dir in blue
                if (S_ISDIR(statbuf.st_mode)) 
                {
                    printf("\033[1;34m %s\033[0m\n", entry_list[i]->d_name);
                } 
                // executable in green
                else if (statbuf.st_mode & S_IXUSR) 
                {
                    printf("\033[1;32m %s\033[0m\n", entry_list[i]->d_name);
                }
                // files in white
                else 
                {
                    printf("\033[1;37m %s\033[0m\n", entry_list[i]->d_name);
                }
            }
        }
    }
    else if (flag==2)
    {
        for (int i = 0; i < n; i++) 
        {
            snprintf(fullpath, sizeof(fullpath), "%s/%s", name_of_dir, entry_list[i]->d_name);
            if (stat(fullpath, &statbuf) == 0) 
            {
                printf((S_ISDIR(statbuf.st_mode)) ? "d" :
                       (S_ISLNK(statbuf.st_mode)) ? "l" : "-");
                printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
                printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
                printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
                printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
                printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
                printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
                printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
                printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
                printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");

                printf(" %ld", (long) statbuf.st_nlink);

                // Print owner and group names
                struct passwd *pw = getpwuid(statbuf.st_uid);
                struct group *gr = getgrgid(statbuf.st_gid);
                printf(" %s %s", pw->pw_name, gr->gr_name);

                // Print file size
                printf(" %ld", (long) statbuf.st_size);

                // Print last modification time
                char timebuf[80];
                struct tm lt;
                localtime_r(&statbuf.st_mtime, &lt);
                strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", &lt);
                printf(" %s", timebuf);

                // dir in blue
                if (S_ISDIR(statbuf.st_mode)) 
                {
                    printf("\033[1;34m %s\033[0m\n", entry_list[i]->d_name);
                } 
                // executable in green
                else if (statbuf.st_mode & S_IXUSR) 
                {
                    printf("\033[1;32m %s\033[0m\n", entry_list[i]->d_name);
                }
                // files in white
                else 
                {
                    printf("\033[1;37m %s\033[0m\n", entry_list[i]->d_name);
                }
            }
        }
    }

    // Free memory
    for (int i = 0; i < n; i++) 
    {
        free(entry_list[i]);
    }
    free(entry_list);
    free(name_of_dir);
    closedir(dir);
}

