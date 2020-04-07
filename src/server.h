#ifndef __SERVER_H__
#define __SERVER_H__

#include "csapp.h"

#define MAX_NAME_LEN 256


/* Handlers used to kill zombie slaves */


void handler_kill(int sig);

void handler_fork(int sig);


/* Server functions */

/**
 * the main program excuted by eache slave (Accept excuter)
 * 
 * @param fd: connection file descriptor
 * 
 **/
void listen_slave(int listenfd);

/**
 * send response of the received reqeust,
 * useually it contains the command code,
 * and data about the commande
 * 
 * @param buff: response text
 * @param n: length of response text
 * @param fd: file descriptor of the connection
 * 
 **/
void send_res(char *buff, size_t n, int cmd_code, int fd);


/**
 * reqeust handler, it handels the reqeusr received from the client,
 * it checks the reqeust and send back the required response/data
 * 
 * @param req: received reqeust
 * @param fd: connection file descriptor to send data (uses Rio_writen())
 * @return handled commande code
 * 
 **/
int handel_req(char *req, int fd);


#endif