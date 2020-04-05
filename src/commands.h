#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "csapp.h"

#define MAXARGS 10

#define GET "get"
#define GETV 1
#define LS  "ls"
#define LSV  2
#define PWD "pwd"
#define PWDV 3
#define CD  "cd"
#define CDV  4
#define MKD "mkdir"
#define MKDV 5
#define RM  "rm"
#define RMV  6
#define RMR "-r"
#define RMRV 7
#define PUT "put"
#define PUTV 8
#define EXT "exit"
#define EXTV 9

/**
 * Check if cmd is a valid command
 * 
 * @param cmd: command to test
 * @return CODEV if the command if it is valid, 0 otherwise.
 * 
 **/
int is_valid_cmd(const char *cmd);


/**
 * excute the get command on arguments 
 * 
 * 
 * @param arg: argument (file name) to send to client 
 * @return file size
 *
 **/
int get(const char* file_name, int fd);

/**
 * excute get command handler for client to receive file
 * 
 * 
 * @param res: response text (file name usually)
 * @param fds: file descripto rio_t structur of the connection
 * @return received bytes
 * 
 **/
int get_client(const char* res, rio_t fds);




#endif