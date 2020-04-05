#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "csapp.h"


void send_req(char *buff, size_t n, int fd);

int get_res(char *buff, size_t n, rio_t fds);

void print_not_found_err(char *cmd);

void handel_res(char *res, rio_t fds);






#endif