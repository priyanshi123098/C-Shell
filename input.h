#ifndef INPUT_H
#define INPUT_H
int split_input(char full_comm[], char dir_of_file[], char* prev_rel_path);
int pipe_with_fgtime(char full_comm[], char dir_of_file[], char *prev_rel_path, int *log_flag);
#endif