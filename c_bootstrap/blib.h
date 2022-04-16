/* B library in c, implemented from K. Thompson's B manual */
/* wrc 20220405 */

#ifndef _BLIB_H_
#define _BLIB_H_

#include "definitions.h"

/* ***********************************************************************
 * From the Ken Thompson manual (section 8.0)
 * 
 * There is a library of B functions maintained in the file /etc/libb.a 
 * The following is a list of those functions currently in the library.
 * See section II of [4] for complete descriptions of the functions 
 * marked with an *.
 * c = char(string, i);  The i-th character of the string is returned.
 * error = chdir(string) ; 
 * error = chmod(string, mode);                                     (*)
 * error = chown(string, owner);                                    (*)
 * error = close(file) ;                                            (*) 
 * file = creat(string, mode);                                      (*)
 * ctime(time, date);              
 * execl(string, arg0, arg1, ..., 0);                               (*)
 * execv(string, argv, count);                                      (*)
 * exit( ) ;                     The current process is terminated. (*)
 * error = fork( ) ;                                                (*)
 * error = fstat(file, status);                                     (*)
 * char = getchar( ) ;
 * id = getuid();                                                   (*)
 * error = gtty(file, ttystat); teletype modes  is returned         (*)
 * lchar(string, i, char);  char stored in i-th character of string.
 * error = link(string1, string2); path string2 created link to  string1 (*)
 * error = mkdir(string, mode);                                     (*)
 * file = open(string, mode);                                       (*)
 * printf(format, argl, ...);  See section 9.3 below.
 * printn(number, base);       See section 9.1 below.
 * putchar(char) ; 
 * nread = read(file, buffer, count);                               (*)
 * error = seek(file, offset, pointer);                             (*)
 * error = setuid(id);                                              (*)
 * error = stat(string, status); i-node of file in vector status    (*)
 * error = stty(file, ttystat);                                     (*)
 * time(timev);  current system time returned in vector timev.      (*)
 * error = unlink(string);                                          (*)
 * error = wait( );   proc suspended until a child proc terminates  (*)
 * nwrite = write(file, buffer, count);                             (*)
 ********************************************************************** */

word b_char(word str, word i);  /*The i-th character of the string is returned. */
word b_chdir(word str) ; 
word b_chmod(word str, word mode);
word b_chown(word str, word owner);
word b_close(word f);
word b_creat(word str, word mode);
void b_ctime(word time, word date);
void b_execl(word str, word arg0);
void b_execv(word str, word argvec, word count);
void b_exit(void);                 /*  The current process is terminated.*/
word b_fork(void);
word b_fstat(word f, word status);
word b_getchar(void);
word b_getuid(void);
word b_gtty(word f, word ttystat); /* teletype modes  is returned */
void b_lchar(word str, word i, word ch); /* ch stored in i-th char of str */
word b_link(word str1, word str2); /* path str2 created link to  str1 */
word b_mkdir(word str, word mode);
word b_open(word str, word mode);
void b_printf(word format/* , word argl, ... */); 
void b_printn(word number, word base);
void b_putchar(word ch); 
word b_read(word f, word buffer, word count);
word b_seek(word f, word offset, word pointer);
word b_setuid(word id);
word b_stat(word str, word status); /* i-node of file into vector status */
word b_stty(word f, word ttystat);
void b_time(word timev);  /* current system time returned in vector timev */
word b_unlink(word str);
word b_wait(void);   /* proc suspended until a child proc terminates */
word b_write(word f, word buffer, word count);



/* ctype.h */
word b_isalnum(word ch);
word b_isalpha(word ch);
word b_isascii(word ch); /* avr? */
word b_isblank(word ch); /* avr? */
word b_iscntrl(word ch);
word b_isdigit(word ch);
word b_isgraph(word ch);
word b_islower(word ch);
word b_isprint(word ch);
word b_ispunct(word ch);
word b_isspace(word ch);
word b_isupper(word ch);
word b_isxdigit(word ch);

word b_toascii(word ch);
word b_tolower(word ch);
word b_toupper(word ch);


/* string.h */


#endif /* _BLIB_H_ */

