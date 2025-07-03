#ifndef LOG_H
#define LOG_H
int log_counter(char log_file[]);
void log_show (char dir_of_file[]);
void log_append (char full_comm[], char dir_of_file[]);
void log_purge (char dir_of_file[]);
void log_execute (char full_comm[], char dir_of_file[],  char* prev_rel_path);

#endif
