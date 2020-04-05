#include "client.h"
#include "csapp.h"
#include "commands.h"
#include "util.h"

int main(int argc, char **argv)
{
    int clientfd, port, cmd_val;
    char *host, input[MAXLINE], buff[MAXLINE];
    size_t n;
    rio_t rio;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = atoi(argv[2]);

    /*
     * Note that the 'host' can be a name or an IP address.
     * If necessary, Open_clientfd will perform the name resolution
     * to obtain the IP address.
     */
    clientfd = Open_clientfd(host, port);
    
    /*
     * At this stage, the connection is established between the client
     * and the server OS ... but it is possible that the server application
     * has not yet called "Accept" for this connection
     */

    // TODO: print connection data
    printf("client connected to server OS\n"); 
    
    Rio_readinitb(&rio, clientfd);

    while (Fgets(input, MAXLINE, stdin) != NULL) {

        // send req
        printf("input: %s\n", input);
        send_req(input, strlen(input), clientfd);
        // Rio_writen(clientfd, input, strlen(input));

        // get res
        cmd_val = get_res(buff, MAXLINE, rio);
        printf("response: %d %s\n", cmd_val, buff);

        // handel response
        handel_res(buff, rio);
        // get_client("./received", rio);
        printf("done\n");
        break;
    }


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
    Rio_readlineb(&fds, buff, n);

    return is_valid_cmd(buff);
}

void handel_res(char *res, rio_t fds) {
    int cmd_code, args;
    char **res_data;


    cmd_code = is_valid_cmd(res);
    args = str_split(res, " ", &res_data);

    printf("res data[0] = %s, cmod %d\n", res, cmd_code);

    switch (cmd_code)
    {
    case GETV:
        get_client(res, fds);
        break;
    
    case LSV:
        printf("commande LS\n");
        break;

    case PWDV:
        printf("commande LS\n");
        break;

    case CDV:
        printf("commande LS\n");
        break;

    case RMV:
        printf("commande LS\n");
        break;

    case PUTV:
        printf("commande LS\n");
        break;

    case EXTV:
        printf("commande LS\n");
        break;

    default:
        print_not_found_err(res_data[0]);
        break;
    }
}
