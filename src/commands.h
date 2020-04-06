#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "csapp.h"

#define MAXARGS 10


/**
 * 
 * commands code, used to comminecate between server-client
 * a commun usage is to tecognize which response to handel
 * (how to behaves with the responses)
 * 
 **/

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
 * @return CODE of the command if it is valid, 0 otherwise.
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


/**
 * Run linux commande using execvp, used fo ls/pwd commands
 * 
 * 
 * @param cmd: linux commande
 * @return output of commande
 * 
 **/
char* run_cmd(char *cmd);


/**
 * remove a set of files
 * 
 * 
 * @param file_names: file names to remove
 * @return response text, contains info about deleted files
 * 
 **/
char* remove_file(const char *file_name);


/**
 * remove a set of empty directories
 * 
 * 
 * @param file_names: directories names to remove
 * @return response text, contains info about deleted directories
 * 
 **/
char* remove_dir(const char *req);

/**
 * print file to stdout
 * 
 * 
 * @param req: request, it contains file names.
 * @return response text (details about displayed files)
 **/
char* put_file(char *req);


#endif