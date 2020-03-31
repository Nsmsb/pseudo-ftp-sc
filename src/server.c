/*
 * echoserveri.c - An iterative echo server
 */

#include "server.h"
#include "csapp.h"

pid_t procss[NBPROC];

void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("server received %u bytes\n", (unsigned int)n);
        Rio_writen(connfd, buf, n);
    }
}

void listen_slave(int listenfd)
{
    int connfd;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN];
    char client_hostname[MAX_NAME_LEN];

    clientlen = (socklen_t)sizeof(clientaddr);


    while (1) {
            
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

            printf("excuter with pid %d is running\n", getpid());

            
            /* determine the name of the client */
            Getnameinfo((SA *) &clientaddr, clientlen,
                        client_hostname, MAX_NAME_LEN, 0, 0, 0);
            
            /* determine the textual representation of the client's IP address */
            Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
                    INET_ADDRSTRLEN);
            
            printf("server connected to client %s (%s)\n", client_hostname,
                client_ip_string);

            echo(connfd);

            printf("closing connfd by child\n");
            Close(connfd); 
            
            }
    
}


/* 
 * Note that this code only works with IPv4 addresses
 * (IPv6 is not supported)
 */
int main(int argc, char **argv)
{
    int listenfd, port;
    pid_t child_pid;

    // setting a handler for SIGINT
    Signal(SIGINT, handler_kill);
    
    // checking if parameters were passed (port)
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    // getting the port
    port = atoi(argv[1]);
    

    listenfd = Open_listenfd(port);

    // creating slaves

    for (int i = 0; i < NBPROC; i++)
    {
        child_pid = Fork();
        if(child_pid == 0)
        {
            printf("child %d created\n", getpid());
            Signal(SIGINT, handler_fork);
            break;
        }
        procss[i] = child_pid;
    }

    

    if(child_pid == 0)
        listen_slave(listenfd);
        
    while (1);
    
    
    exit(0);
}

void handler_kill(int sig) {
    printf("\nTerminating..\n");
    Sleep(2);
	for(int i=0; i<NBPROC; i++){
		Kill(procss[i], SIGINT);
	}
	exit(0);
}


void handler_fork(int sig) {
    Sleep(1);
    printf("slave with pid %d is terminated\n", getpid());
    exit(0);
}