#include "headers.h"
void hop (char dir_of_file[], char path[], char* prev_rel_path) 
{
    char delim[] = " "; 
    char *token = strtok(path, delim);
    token = strtok(NULL, delim); 

    char* name_of_dir= (char*)malloc (4096 * sizeof (char));
    char curr_dir_path [4096];
    if (getcwd(curr_dir_path, 4096) == NULL)
    {
        perror("cwd path not obtained");
        return;
    }
    if (token==NULL)
    {
        strcpy (name_of_dir, dir_of_file);
        if (chdir (name_of_dir)!=0) {
            perror ("couldn't change dir");
            return;
        }
        printf ("%s\n", name_of_dir);
        return;
    }
    else
    {
        strcpy (name_of_dir, token);
        if (name_of_dir != NULL)
        {
            while (*name_of_dir == ' ' || *name_of_dir == '\t')
                name_of_dir++;
        }
    }

    if (name_of_dir==NULL || (strcmp (name_of_dir, "~")==0)) {
        strcpy (name_of_dir, dir_of_file);
        if (chdir (name_of_dir)!=0) {
            perror ("couldn't change dir");
            return;
        }
        printf ("%s\n", name_of_dir);
        return;;
    }
    while (token != NULL) {        
        strcpy (name_of_dir, token);
        // path of dir is given starting with'/'
        if (name_of_dir==NULL || (strcmp (name_of_dir, "~")==0)) 
        {
            strcpy (name_of_dir, dir_of_file);
            if (chdir (name_of_dir)!=0) {
                perror ("couldn't change dir");
                return;
            }
        }
        else if (name_of_dir[0]=='/') {
            if (chdir (name_of_dir)!=0) {
                perror ("couldn't change dir");
                return;
            }
        }
        // if only ~ is there: home dir
        // starting with ~/name_of_dir
        else if (name_of_dir[0]=='~') {
            int k= strlen (name_of_dir);
            char *startPos = name_of_dir + 2;
            strncpy(name_of_dir, startPos, k-2);
            name_of_dir[k-2] = '\0';
            char temp[4096]="/";
            strcat (temp, name_of_dir);
            strcpy (name_of_dir, curr_dir_path);
            strcat (name_of_dir, temp);
            if (chdir (name_of_dir)!=0)  {
                perror ("couldn't change dir");
                return;
            }
        }
        else if (strcmp (name_of_dir, "-")==0)
        {
            strcpy (name_of_dir, prev_rel_path);
            if (chdir (name_of_dir)!=0)  {
                perror ("couldn't change dir");
                return;
            }
        }
        // only name of dir is given
        else {
            char temp[4096]="/";
            strcat (temp, name_of_dir);
            strcpy (name_of_dir, curr_dir_path);
            strcat (name_of_dir, temp);
            if (chdir (name_of_dir)!=0)  {
                perror ("couldn't change dir");
                return;
            }
        }
        if (getcwd (curr_dir_path, 4096)==NULL)
        {
            perror("cwd path not obtained");
            return;
        }
        printf ("%s\n", curr_dir_path);
        token = strtok(NULL, delim);
    }
    return;
}