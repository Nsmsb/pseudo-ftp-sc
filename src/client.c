#include "client.h"
#include "csapp.h"
#include "config.h"
#include "commands.h"
#include "util.h"

int main(int argc, char **argv)
{
    int clientfd, cmd_code;
    char *host, *input, *buff;
    input = malloc(MAXLINE);
    buff = malloc(MAXLINE);
    // size_t n;
    rio_t rio;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <host>\n", argv[0]);
        exit(0);
    }
    host = argv[1];

    /*
     * Note that the 'host' can be a name or an IP address.
     * If necessary, Open_clientfd will perform the name resolution
     * to obtain the IP address.
     */
    clientfd = Open_clientfd(host, PORTFTP);
    
    /*
     * At this stage, the connection is established between the client
     * and the server OS ... but it is possible that the server application
     * has not yet called "Accept" for this connection
     */

    // TODO: print connection data
    printf("client connected to server OS\n"); 
    
    // initializing the rio struct
    Rio_readinitb(&rio, clientfd);

    // cmdd_code current commande to excute code, check commands.h
    cmd_code = 0;

    // while EXIT command is not entred, take input from use
    while (cmd_code != EXTV) {

        // initialising buffers (allocating)
        input = malloc(MAXLINE);
        buff = malloc(MAXLINE);

        // reading input from user

        printf("%sftp>%s ", COL_GRN, COL_RST);
        if(Fgets(input, MAXLINE, stdin) == NULL) {
            print_error("client is dead, terminating..\n");
            exit(0);
        }

        if(strlen(input) <= 1)
            continue;


        // send req
        send_req(input, strlen(input), clientfd);
        // Rio_writen(clientfd, input, strlen(input));

        // get res
        cmd_code = get_res(buff, MAXLINE, rio);

        // handel response
        handel_res(buff, cmd_code, rio);

        // free buffers and allocating them
        Free(input);
        Free(buff);
        input = Malloc(MAXLINE);
        buff = Malloc(MAXLINE);


    }

    printf("%sdisconnecting..%s\n",COL_RED, COL_RST);
    Close(clientfd);
    exit(0);
}


void print_not_found_err(char *cmd)
{
    printf("%s: %scommand not found%s\n", cmd, COL_RED,COL_RST);
}


void send_req(char *buff, size_t n, int fd)
{
    Rio_writen(fd, buff, n);
}



int get_res(char *buff, size_t n, rio_t fds) {
    int cmd_code;

    Rio_readnb(&fds, &cmd_code, sizeof(cmd_code));
    Rio_readlineb(&fds, buff, n);

    return cmd_code;
}



int handel_res(char *res, int cmd_code, rio_t fds) {
    int args;
    char *file;
    char **res_data;
    
    args = str_split(res, " ", &res_data);

    switch (cmd_code)
    {
    case GETV:
        // fmultiples files are divided into sigle files reqeusts
        for (int i = 1; i < args; i++)
        {
            printf("transfering file: %s\n", res_data[i]);

            // parsing file name
            file = parse_file_name(res_data[i], NULL);
            // receiving file
            get_client(file, fds);
            Free(file);
        }
        break;
    
    case LSV:
        // response contains data
        printf("%s", res);
        break;

    case PWDV:
        // response contains data
        printf("%s", res);
        break;

    case CDV:
        // response contains data
        printf("%s", res);
        break;

    case RMV:
        // res contains fetails about deleted files
        printf("%s", res);
        break;

    case RMRV:
        // res contains fetails about deleted files
        printf("%s", res);
        break;

    case PUTV:
        // res contains fetails about deleted files
        printf("%s", res);
        break;

    case EXTV:
        printf("%s", res);
        break;

    default:
        print_not_found_err(res_data[0]);
        break;
    }

    return cmd_code;
}
