#include <string.h>
#include "commands.h"
#include "csapp.h"

int is_valid_cmd(const char *cmd) {

    if(strncmp(cmd, GET, sizeof(GET)-1) == 0)
        return GETV;
    if(strncmp(cmd, PUT, sizeof(PUT)-1) == 0)
        return PUTV;
    if(strncmp(cmd, LS, sizeof(LS)-1) == 0)
        return LSV;
    if(strncmp(cmd, PWD, sizeof(PWD)-1) == 0)
        return PWDV;
    if(strncmp(cmd, CD, sizeof(CD)-1) == 0)
        return CDV;
    if(strncmp(cmd, MKD, sizeof(MKD)-1) == 0)
        return MKDV;
    if(strncmp(cmd, RM, sizeof(RM)-1) == 0)
        return RMV;
    if(strncmp(cmd, RMR, sizeof(RMR)-1) == 0)
        return RMRV;
    if(strncmp(cmd, EXT, sizeof(EXT)-1) == 0)
        return EXTV;
    
    return 0;
}


int get(const char* file_name, int fd) {
    size_t sent_bytes, n, file_size;
    int file_fd;
    char buf[MAXBUF];
    // char fsize[MAXLINE];
    rio_t rio;

    file_fd = Open(file_name, O_RDONLY, 0);

    Rio_readinitb(&rio, file_fd);
    sent_bytes = 0;

    // calculating file size
    file_size = (size_t) Lseek(file_fd,0,SEEK_END);
    // sprintf(fsize, "%ld", n);
    Lseek(file_fd,0,SEEK_SET);
    printf("file size: %ld\n", file_size);

    // sending file size
    Rio_writen(fd, &file_size, sizeof(file_size));

    printf("size sent\n");
    
    
    while ((n = Rio_readnb(&rio, buf, MAXBUF)) > 0) {
        Rio_writen(fd, buf, n);
        // printf("line: %s", buf);
        sent_bytes += n;
    }
    Close(file_fd);
    printf("done\n");
    return sent_bytes;
}


int get_client(const char* res, rio_t fds) {
    int file_fd;
    size_t n, res_bytes, file_size;
    time_t start, end;
    unsigned long ttime;
    char buf[MAXBUF];


    res_bytes = 0;
    file_fd = Open(res, O_WRONLY | O_CREAT | O_APPEND, 0666);

    // get file size
    printf("reading file size\n");

    Rio_readnb(&fds,&file_size, sizeof(size_t));
    printf("file size: %ld\n", file_size);

    // transfer start
    start = time(NULL);

    while (file_size > 0)
    {
        printf("start\n");
        if(file_size < MAXBUF)
            n = Rio_readnb(&fds,buf, file_size);
        else
            n = Rio_readnb(&fds,buf, MAXBUF);

        Rio_writen(file_fd, buf, n);
        // printf("data: %s\n", buf);
        res_bytes += n;
        file_size -= n;
    }

    // file transfer end
    end = time(NULL);

    Close(file_fd);

    ttime = difftime(end, start);

    printf("received %ld bytes in %ld\n", res_bytes, (unsigned long)ttime);
    
    
    return res_bytes;
}