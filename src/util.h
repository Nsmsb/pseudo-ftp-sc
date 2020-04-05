#ifndef __UTIL__H_
#define __UTIL__H_

#define COL_RST  "\x1B[0m"
#define COL_BLD  "\x1B[1m"
#define COL_RED  "\x1B[31m"
#define COL_GRN  "\x1B[32m"
#define COL_YEL  "\x1B[33m"
#define COL_BLU  "\x1B[34m"
#define COL_MAG  "\x1B[35m"
#define COL_CYN  "\x1B[36m"
#define COL_WHT  "\x1B[37m"

/**
 * Print an error message on stderr
 *
 * @param format: a string which contains format
 * @return the number of characters printed
 *
 **/
int print_error(const char *format, ...);

/**
 * Print an error message on stderr and add a string describing error number
 * used to print errno string text.
 *
 * @param format: a string which contains format
 * @return the number of characters printed
 *
 **/
int print_error_errno(const char *format, ...);

/**
 * Split a string using a delimiter into an array of strings
 * used parse input
 *
 * @param src: strinf to print
 * @param delims: delimiter
 * @param dest: destination (board effect)
 * @return the number of words in string
 *
 **/
int str_split(const char* src, const char* delims, char*** dest);



#endif