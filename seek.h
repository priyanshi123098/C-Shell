#ifndef SEEK_H
#define SEEK_H
void seek(char full_comm[], char dir_of_file[], char* prev_rel_path);
void append_matches(char search[], char curr_path[], int flag_d, int flag_e, int flag_f, 
            char initial_target_dir[], char** dir, char** file, int *idx1, int *idx2);
void print (char full_path[], int col, char initial_target_dir[]);       


#endif