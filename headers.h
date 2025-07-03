#ifndef HEADERS_H
#define HEADERS_H
#include <stdio.h>
#include <sys/utsname.h>
#include <signal.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <math.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <termios.h>
#include <sys/select.h>
#include "input.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "sys_comm.h"
#include "proclore.h"
#include "seek.h"
#include "redirection.h"
#include "activities.h"
#include "pipes.h"
#include "signals.h"
#include "iman.h"
#include "fg_bg.h"
#include "neonate.h"
typedef struct info {
    char command[4096];
    int pid;
    char state[16];
}info;
extern info** act_list;
extern int num_act_list;
extern pid_t last_fg_pid;
extern char* last_fg_name;
extern struct termios original_tio;
#endif
