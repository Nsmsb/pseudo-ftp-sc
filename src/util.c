#include "util.h"
#include "commands.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>


/* error handling utilities */

int print_error(const char *format, ...)
{
	int ret;
	va_list aptr;

	va_start(aptr, format);
	fprintf(stderr, "%s%sERROR:%s ", COL_BLD, COL_RED, COL_RST);
	ret = vfprintf(stderr, format, aptr);
	fprintf(stderr, "\n");
	va_end(aptr);

	return ret;
}

int print_error_errno(const char *format, ...)
{
	int ret;
	va_list aptr;

	va_start(aptr, format);
	fprintf(stderr, "%s%sERROR:%s ", COL_BLD, COL_RED, COL_RST);
	ret = vfprintf(stderr, format, aptr);
	fprintf(stderr, ": %s\n", strerror(errno));
	va_end(aptr);

	return ret;
}


/* string parsing */

char* parse_file_name(char *name, char *path)
{
    int name_size = strlen(name);
    char *result = malloc(name_size);
    strcpy(result, name);

    if(path == NULL)
        path = "./";

    if(strncmp("./", name, 2) != 0) {
        result = Realloc(result, name_size + 2);
        sprintf(result, "%s%s",path, name);
    }

    char *pos;
    if ((pos=strchr(result, '\n')) != NULL)
    *pos = '\0';

    
    return result;
}

int str_split(const char* src, const char* delims, char*** dest) {
    char *s = strdup(src);
    char *c, *saveptr, *tmp;
    int num_tokens = 0, num_size = 10;
    c = strtok_r(s, delims, &saveptr);
    if (c == NULL)
        return 0;
        
    *dest = Malloc(num_size * sizeof(char*));
    
    tmp = strdup(c);
    if (tmp == NULL) {
        free(*dest);
        free(s);
        *dest = NULL;
        return -1;
    }
    
    (*dest)[num_tokens] = tmp;
    num_tokens++;
    
    while ((c = strtok_r(NULL, delims, &saveptr)) != NULL) {
        if (num_tokens == num_size) {
            num_size += 10;
            void* _tmp = Realloc(*dest, num_size*sizeof(char*));
            *dest = (char**)_tmp;
        }
        tmp = strdup(c);
        if (tmp == NULL) {
            free(*dest);
            free(s);
            *dest = NULL;
            return -1;
        }
        
        (*dest)[num_tokens] = tmp;
        num_tokens++;
    }

    return num_tokens;
}
