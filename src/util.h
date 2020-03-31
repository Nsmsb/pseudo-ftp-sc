#ifndef _UTIL_H_
#define _UTIL_H_


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