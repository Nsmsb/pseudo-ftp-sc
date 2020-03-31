#ifndef __SERVER_H__
#define __SERVER_H__

#define MAX_NAME_LEN 256
#define NBPROC 2


/* Handlers used to kill zombie slaves */

void handler_kill(int sig);

void handler_fork(int sig);


/* Server functions */

void listen_slave(int listenfd);


void echo(int connfd);









#endif