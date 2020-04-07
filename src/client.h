#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "csapp.h"


/**
 * responser handler, handel the response received from server,
 * it check commands to handel their response,
 * prints erro received from server
 * 
 * @param res: response to handel
 * @param cmd_code: commande code (see commands.h)
 * @param fds: rio file descriptor struct
 * @return handled command code
 * 
 **/
int handel_res(char *res, int cmd_code, rio_t fds);


/**
 * send req data to server
 * 
 * 
 * @param buff: contains reqeust
 * @param n: size of data to send
 * @param fd: connection file descriptor returned by Accept()
 * @return None
 * 
 **/
void send_req(char *buff, size_t n, int fd);


/**
 * wait/get response data sent by server, data is in format of a line
 * 
 * 
 * @param buff: buffer wher to store data
 * @param n: size of data to receive (max size)
 * @param fds: rio file descriptor struct
 * @return commande code (see commands.h)
 * 
 **/
int get_res(char *buff, size_t n, rio_t fds);


/**
 * print an error message to tell client about an invalid commande
 * 
 * @param cmd: not found commande
 **/
void print_not_found_err(char *cmd);


#endif