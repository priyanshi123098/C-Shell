#ifndef SYS_COMM_H
#define SYS_COMM_H
void sigchld_handler(int sig);
void execute_command_background(char *input, char dir_of_file[]);
int execute_command_foreground(char *input, char dir_of_file[]);
int sys_comm(char full_comm[], char dir_of_file[]);

#endif
