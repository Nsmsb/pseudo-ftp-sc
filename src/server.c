#include <string.h>
#include "server.h"
#include "config.h"
#include "csapp.h"
#include "commands.h"
#include "util.h"

pid_t procss[NBPROC];

char current_path[MAXLINE];//"./";



int main(int argc, char **argv)
{
    int listenfd;
    pid_t child_pid;

    // setting a handler for SIGINT
    Signal(SIGINT, handler_kill);
    

    listenfd = Open_listenfd(PORTFTP);

    strcpy(current_path, "./");

    
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


void listen_slave(int listenfd)
{
    int connfd;
    size_t n;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    rio_t input_str;
    char client_ip_string[INET_ADDRSTRLEN];
    char client_hostname[MAX_NAME_LEN];
    char *input;

    clientlen = (socklen_t)sizeof(clientaddr);


    while (1) {
            
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

            printf("Slave with pid %d established connection\n", getpid());

            
            /* determine the name of the client */
            Getnameinfo((SA *) &clientaddr, clientlen,
                        client_hostname, MAX_NAME_LEN, 0, 0, 0);
            
            /* determine the textual representation of the client's IP address */
            Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
                    INET_ADDRSTRLEN);
            
            printf("server connected to client %s (%s)\n", client_hostname,
                client_ip_string);

            // initializing rio struct to read input from client
            Rio_readinitb(&input_str, connfd);

            // initializing
            input = malloc(MAXLINE);

            // reading input
            while ((n = Rio_readlineb(&input_str, input, MAXLINE)) != 0)
            {

                printf("%sclient@%d>%s %s", COL_CYN, getpid(), COL_RST, input);

                input[n-1] = '\0';

                // sending response data
                handel_req(input, connfd);

                // reallocating input
                free(input);
                input = malloc(MAXLINE);

            }
            

            printf("%sclinet disconnected:%s  %s(%s)\n", COL_RED, COL_RST,client_hostname, client_ip_string);
            Close(connfd); 
            
            }
    
}


void send_res(char *buff, size_t n, int cmd_code, int fd) {
    char nl = '\n';

    Rio_writen(fd, &cmd_code, sizeof(int));
    Rio_writen(fd, buff, n);
    if(buff[n-1] != '\n')
        Rio_writen(fd, &nl, 1);
}

int handel_req(char *req, int fd) {
    int cmd_code, args;
    char *res, *file;
    char **req_data;

    // deleting \n from res string
    // req[strlen(req) -2] = '\0';

    cmd_code = is_valid_cmd(req);

    args = str_split(req, " ", &req_data);


    switch (cmd_code)
    {
    case GETV:
        send_res(req, strlen(req), GETV, fd);

        for (int i = 1; i < args; i++)
        {
            printf("getting file %s\n", req_data[i]);
            file = parse_file_name(req_data[i], current_path);
            get(file, fd);
            Free(file);
        }

        // get("./testpng", fd);
    
        break;
    
    case LSV:
        res = run_cmd(req);
        send_res(res, strlen(res), cmd_code, fd);
        free(res);

        break;

    case PWDV:
        res = run_cmd(req);
        send_res(res, strlen(res), cmd_code, fd);
        free(res);
        break;

    case CDV:
        res = cd_dir(req, current_path);
        send_res(res, strlen(res), CDV, fd);
        break;

    case RMV:
        res = remove_file(req, current_path);
        send_res(res, strlen(res), cmd_code, fd);
        free(res);
        break;

    case RMRV:
        res = remove_dir(req, current_path);
        send_res(res, strlen(res), cmd_code, fd);
        free(res);
        break;

    case PUTV:
        res = put_file(req, current_path);
        send_res(res, strlen(res), cmd_code, fd);
        free(res);
        break;

    case EXTV:
        res = "server closed connection.";
        send_res(res, strlen(res), cmd_code, fd);
        break;

    default:
        printf("invalid found command, a response will be sent\n");
        res = "invalid commande";
        send_res(res, strlen(res), cmd_code, fd);
    }

    return cmd_code;
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