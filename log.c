#include "headers.h"
// Function to count the number of lines in the log file
int log_counter(char log_file[]) 
{
    FILE *file = fopen(log_file, "r");
    if (file == NULL) 
    {
        perror("Unable to open file");
        return -1; 
    }
    int count = 0;
    char buffer[4096]; // Buffer to hold each line
    while (fgets(buffer, sizeof(buffer), file) != NULL) 
    {
        count++; 
    }
    fclose(file);
    return count;
}

void log_show(char dir_of_file[]) 
{
    char log_file[4096];
    snprintf(log_file, sizeof(log_file), "%s/log_file.txt", dir_of_file);

    FILE *file = fopen(log_file, "r");
    if (file == NULL) 
    {
        perror("Error opening file");
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch); 
    }
    fclose(file);
}

void log_append(char full_comm[], char dir_of_file[]) {
    char **strings = (char **)malloc(sizeof(char *) * 15);
    for (int i = 0; i < 15; i++) 
    {
        strings[i] = (char *)malloc(4096 * sizeof(char));
        if (strings[i] == NULL) 
        {
            fprintf(stderr, "Memory allocation failed for string %d\n", i);
            return;
        }
    }

    char log_file[4096];
    snprintf(log_file, sizeof(log_file), "%s/log_file.txt", dir_of_file);

    FILE *file = fopen(log_file, "r");
    if (file == NULL) 
    {
        perror("Unable to open file");
        return; 
    }

    int log_count = log_counter(log_file);

    char buffer[4096]; // Buffer to hold each line
    int i = 0;
    while (i < log_count && fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';  
        strncpy(strings[i], buffer, 4096);
        i++;
    }
    fclose(file);

    if (log_count < 15) 
    {
        if (log_count == 0 || (log_count > 0 && strcmp(full_comm, strings[log_count - 1]) != 0)) 
        {
            file = fopen(log_file, "a");
            if (file == NULL) 
            {
                perror("Failed to open the file");
                return;
            }
            fprintf(file, "%s\n", full_comm);
            fclose(file);
        }
    }
    else 
    {
        if (strcmp(full_comm, strings[log_count - 1]) != 0) 
        {
            for (int i = 1; i < 15; i++) 
            {
                strncpy(strings[i - 1], strings[i], 4096);
            }
            strncpy(strings[14], full_comm, 4096);

            file = fopen(log_file, "w");
            if (file == NULL) 
            {
                perror("Failed to open the file");
                return;
            }
            for (int i = 0; i < 15; i++) 
            {
                fprintf(file, "%s\n", strings[i]);
            }
            fclose(file);
        }
    }

    for (int i = 0; i < 15; i++) 
    {
        free(strings[i]);
    }
    free(strings);
}

void log_purge(char dir_of_file[]) 
{
    char log_file[4096];
    snprintf(log_file, sizeof(log_file), "%s/log_file.txt", dir_of_file);

    FILE *file = fopen(log_file, "w");
    if (file == NULL) {
        perror("Error clearing the log file");
        return;
    }
    fclose(file);
}

void log_execute (char full_comm[], char dir_of_file[], char* prev_rel_path)
{
    char log_file[4096];
    strcpy (log_file, dir_of_file);
    strcat (log_file, "/log_file.txt");
    int log_count= log_counter(log_file);

    char store_full_comm[4096];
    strcpy (store_full_comm, full_comm);
    int index=0;
    char delim[] = " ";
    char *token = strtok(full_comm, delim);
    char temp_to_store_index[4096];
    while (token != NULL)
    {
        strcpy (temp_to_store_index, token);
        token = strtok(NULL, delim);
    }
    index= temp_to_store_index[0]-'0';
    int idx= log_count- index;
    if (idx<0)
    {
        perror ("comm doesn't exist");
        return;
    }
    else
    {
        FILE *filePtr;
        filePtr = fopen(log_file, "r");
        char buffer[4096]; // Buffer to hold each line
        char temp_comm[4096];
        for (int i=0; i<=idx; i++)
        {
            if (fgets(buffer, sizeof(buffer), filePtr)!=NULL)
            {
                buffer[strcspn(buffer, "\n")] = '\0'; 
                strncpy(temp_comm, buffer, 4096);
            }
        }
        split_input(temp_comm, dir_of_file, prev_rel_path);
    }
    return;
}
