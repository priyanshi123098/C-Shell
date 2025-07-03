#include "headers.h"
void print (char full_path[], int col, char initial_target_dir[])        // 0 for blue, 1 for green
{
    char *rel_path;
    if (strstr(full_path, initial_target_dir) != NULL) 
    {
        rel_path= full_path+ strlen (initial_target_dir);
        if (*rel_path == '/') 
        {
            rel_path++;
        }
        if (col==0)     
        printf("\033[1;34m./%s\033[0m\n",rel_path);
        else
        printf("\033[1;32m./%s\033[0m\n",rel_path);
    } 
}
void append_matches(char search[], char curr_path[], int flag_d, int flag_e, int flag_f, 
            char initial_target_dir[], char** dir_temp, char** file, int *idx1, int *idx2)
{
    char path[4096];
    struct dirent *entry;
    struct stat statbuf;
    DIR *dir = opendir(curr_path);
    char fullpath[4096];
    if (dir == NULL)
    {
        perror("opendir");
        exit(1);
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;
        snprintf(fullpath, sizeof(fullpath), "%s/%s", curr_path, entry->d_name); 
        // if it itself matches the search name
        if (strstr (entry->d_name, search)== entry->d_name)
        {
            if (!flag_e) // directories can be printed
            {
                if (flag_d==1)
                {
                    if (stat(fullpath, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
                    {
                        print (fullpath, 0, initial_target_dir);
                        *idx1= *idx1+1;
                    }
                }
                else if (flag_f==1)
                {
                    if (stat(fullpath, &statbuf) == 0 && !S_ISDIR(statbuf.st_mode))
                    {
                        print (fullpath, 1, initial_target_dir);
                        *idx2= *idx2+1;
                    }
                }
                else
                {
                    if (stat(fullpath, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
                    {
                        print (fullpath, 0, initial_target_dir);
                        *idx1= *idx1+1;
                    }
                    else if (stat(fullpath, &statbuf) == 0)
                    {
                        print (fullpath, 1, initial_target_dir);
                        *idx2= *idx2+1;
                    }
                }
            }
            else
            {
                if (flag_d==1)
                {
                    if (stat(fullpath, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
                    {
                        strcpy (dir_temp[*idx1], fullpath);
                        *idx1= *idx1+1;
                    }
                }
                else if (flag_f==1)
                {
                    if (stat(fullpath, &statbuf) == 0 && !S_ISDIR(statbuf.st_mode))
                    {
                        strcpy (file[*idx2], fullpath);
                        *idx2= *idx2+1;
                    }
                }
            }
        }
        // if it is a dir and we need to enter the dir
        if (stat(fullpath, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
        {
            append_matches(search, fullpath, flag_d, flag_e, flag_f, initial_target_dir, dir_temp, file, idx1, idx2);
        }
    }
}
void seek(char full_comm[], char dir_of_file[], char* prev_rel_path)
{
    int flag_d = 0;
    int flag_e = 0;
    int flag_f = 0;
    char delim[] = " ";
    char *token = strtok(full_comm, delim);
    char target_dir[4096];
    char search[4096];
    
    token = strtok(NULL, delim);
    int c = 0;
    while (token != NULL)
    {
        if (strcmp(token, "-d") == 0 || strcmp(token, "-e") == 0 || strcmp(token, "-f") == 0)
        {
            if (strcmp(token, "-d") == 0)
                flag_d = 1;
            else if (strcmp(token, "-e") == 0)
                flag_e = 1;
            else if (strcmp(token, "-f") == 0)
                flag_f = 1;
        }
        else
        {
            if (c == 0)
            {
                strcpy(search, token);
                c++;
            }
            else
            {
                strcpy(target_dir, token);
            }
        }
        token = strtok(NULL, delim);
    }
    // printf ("%d %d %d\n", flag_d, flag_e, flag_f);
    if (c==0)
    {
        if (getcwd(target_dir, 4096) == NULL)
        {
            perror("cwd path not obtained");
            return;
        }
    }
    if (target_dir[0]!='/')
    {
        if (strcmp (target_dir, "~")==0)
        {
            strcpy (target_dir, dir_of_file);
        }
        else if (strcmp (target_dir, "-")==0)
        {
            strcpy (target_dir, prev_rel_path);
        }
        else
        {
            char curr_dir_path[4096];
            if (getcwd(curr_dir_path, 4096) == NULL)
            {
                perror("cwd path not obtained");
                return;
            }
            char temp[4096]="/";
            strcat (temp, target_dir);
            strcpy (target_dir, curr_dir_path);
            strcat (target_dir, temp);
        }
    }
    char* initial_target_dir= (char*)malloc (sizeof (char)* 4096);
    strcpy (initial_target_dir, target_dir);
    if (flag_d==1 && flag_f==1)
    {
        printf ("invalid command\n");
        return;
    }
    char** dir_temp= (char**)malloc (sizeof (char*)* 100);
    char** file= (char**)malloc (sizeof (char*)* 100);
    for (int i=0; i<100; i++)
    {
        dir_temp[i]= (char*)malloc (sizeof (char)* 4096);
        file[i]= (char*)malloc (sizeof (char)* 4096);
    }
    int idx1=0;
    int idx2=0;
    append_matches (search, target_dir, flag_d, flag_e, flag_f, initial_target_dir, dir_temp, file, &idx1, &idx2);
    if (flag_e==1)
    {
        if (flag_d==1)
        {
            if (idx1==0)
            {
                printf ("no dir\n");
                return;
            }
            else if (idx1==1)
            {
                // printf ("%s\n", dir_temp[0]);
                char temp[4096]= "hop ";
                strcat (temp, dir_temp[0]);
                strcpy (dir_temp[0], temp);
                hop (dir_of_file, dir_temp[0], prev_rel_path);
            }
            else
            {
                for (int i=0; i<idx1; i++)
                {
                    print (dir_temp[i], 0, initial_target_dir);
                }
            }
        }
        else if (flag_f==1)
        {
            if (idx2==0)
            {
                printf ("no file\n");
                return;
            }
            else if (idx2==1)
            {
                // print contents of file
                FILE *filePtr;
                filePtr = fopen(file[0], "r");
                if (filePtr == NULL) {
                    perror("Missing permissions for task!");
                    return;
                }
                char ch;
                while ((ch = fgetc(filePtr)) != EOF) 
                {
                    putchar(ch);  // Print the character to the terminal
                }
                fclose(filePtr);
            }
            else
            {
                for (int i=0; i<idx2; i++)
                {
                    print (file[i], 1, initial_target_dir);
                }
            }
        }
    }
    else 
    {
        if (flag_d==1)
        {
            if (idx1==0)
            printf ("No match found!\n");
        }
        else if (flag_f==1)
        {
            if (idx2==0)
            printf ("No match found!\n");
        }
        else
        {
            if (idx1==0 && idx2==0)
            printf ("No match found!\n");
        }
    }
    return;
}