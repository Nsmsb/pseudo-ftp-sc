#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "csapp.h"
#include "util.h"


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

    file_size = -1;
    sent_bytes = 0;
    file_fd = Open(file_name, O_RDONLY, 0);

    // if file not found, send -1 size
    if(fd < 0)
    {
        sprintf(buf, "%ld", file_size);
        buf[strlen(buf)] = '\n';

        Rio_writen(fd, &buf, strlen(buf));
        // Rio_writen(fd, '\n', 1);

        return sent_bytes;
    }

    Rio_readinitb(&rio, file_fd);
    sent_bytes = 0;

    // calculating file size
    file_size = (size_t) Lseek(file_fd,0,SEEK_END);
    // sprintf(fsize, "%ld", n);
    Lseek(file_fd,0,SEEK_SET);
    printf("file size: %ld\n", file_size);

    // sending file size
    sprintf(buf, "%ld", file_size);
    buf[strlen(buf)] = '\n';
    Rio_writen(fd, &buf, strlen(buf));
    // Rio_writen(fd, '\n', 1);
    // Rio_writen(fd, &file_size, sizeof(file_size));

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
    
    char *file_name = malloc(strlen(res) + 5);

    // setting received file name to filename_rcvd
    sprintf(file_name, "%s_rcvd", res); 

    res_bytes = 0;
    file_fd = Open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0666);

    // get file size
    printf("reading file size\n");

    file_size = 0;
    n = Rio_readlineb(&fds, buf, MAXLINE);
    file_size = atoi(buf);
    // n = Rio_readnb(&fds,&file_size, sizeof(size_t));

    // file not found
    if(file_size == 0)
    {
        printf("file not found\n");
        return res_bytes;
    }

    // printf("read %ld, file size: %ld\n", n, file_size);

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

    printf("received %ld bytes (%ld/s)\n", res_bytes, (unsigned long)(ttime == 0 ? 0 : res_bytes/ttime));
    
    
    return res_bytes;
}


char* run_cmd(char *cmd)
{
    char *result = malloc(MAXLINE);
    int pfd[2];
    char **args;
    int n = str_split(cmd, " ", &args);

    // adding las null element for the execvp
    args = (char**) Realloc(args, n+1);
    args[n] = NULL;
    

    pipe(pfd);

    if(Fork() == 0)
    {
        Close(pfd[0]);
        Dup2(pfd[1], 1);
        execvp(args[0], args);
        exit(0);
    }
    Close(pfd[1]);
    Rio_readn(pfd[0], result, MAXLINE);
    wait(NULL);

    return result;
}

char* remove_file(const char *req)
{   
    char *res = malloc(MAXLINE);
    char *file;
    int deleted_files, fn, rm_flag;
    deleted_files = 0;
    char **files_names;
    

    fn = str_split(req, " ", &files_names);

    for (int i = 1; i < fn; i++)
    {
        file = parse_file_name(files_names[i]);
        rm_flag = remove(file);
        
        
        if(rm_flag == 0)
            deleted_files++;
    }

    // writing to response to be returned
    sprintf(res, "deleted %d files.\n", deleted_files);

    return res;
}


char* remove_dir(const char *req)
{   
    char *res = malloc(MAXLINE);
    char *file;
    int deleted_files, fn, rm_flag;
    deleted_files = 0;
    char **files_names;
    

    fn = str_split(req, " ", &files_names);

    for (int i = 1; i < fn; i++)
    {
        file = parse_file_name(files_names[i]);
        rm_flag = rmdir(file);
        
        if(rm_flag == 0)
            deleted_files++;
    }

    // writing to response to be returned
    sprintf(res, "deleted %d directories.\n", deleted_files);

    return res;
}

char* put_file(char *req)
{
    ssize_t n;
    char *buff[MAXBUF];
    char *res = malloc(MAXLINE);
    char *file;
    int displayed_files, fn, fd;
    displayed_files = 0;
    char **files_names;
    

    fn = str_split(req, " ", &files_names);

    for (int i = 1; i < fn; i++)
    {
        file = parse_file_name(files_names[i]);
        fd = Open(file, O_RDONLY, 0);
        if(fd > 0)
        {
            printf("file %s:\n", files_names[i]);
            while ((n = Rio_readn(fd, buff, MAXBUF)) != 0)
                Rio_writen(1, buff, n);
            displayed_files++;
        }
    }

    sprintf(res,"displayed %d file(s)\n", displayed_files);
    return res;
    
}