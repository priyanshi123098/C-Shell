#ifndef SIGNALS_H
#define SIGNALS_H
void sigtstp_handler(int sig);
void handle_sigkill();
void handle_sigint(int sig);
void signals (char full_comm[], char dir_of_file[]);
#endif
