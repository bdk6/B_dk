/* B library in c, implemented from K. Thompson's B manual */
/* wrc 20220405 */

#include <stdio.h>
#include <stdlib.h>

#include "definitions.h"
#include "blib.h"


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

/* ***********************************************************************
 * @fn b_char
 * @brief Retrieves i-th character from string.
 * @param[in] str The string to get character from.
 * @param[in] i The index of character to get.
 * @return The character indexed.
 ********************************************************************** */
word b_char(word str, word i)
{
  word rtn;
  word wrd;
  wrd = i >> 1;
  rtn = str[wrd];
  if( i & 1) /* odd chars are on the right */
  {
    rtn &= 255;
  }
  else       /* even on the left */
  {
    rtn >>= 8;
  }
  return(rtn);
}


/* ***********************************************************************
 * @fn b_chdir
 * @brief Change working directory to string.
 * @param[in] str Name of path/directory to change to.
 * @return 0 or positive on success, negative on fail.
 ********************************************************************** */
word b_chdir(word str)
{
  /* TODO */
  return (-1);
}


/* ***********************************************************************
 * @fn b_chmod
 * @brief
 * @param[inout]
 * @param[inout]
 * @return
 ********************************************************************** */
word b_chmod(word str, word mode)
{
  /* TODO */
  return -1;
}

/* ***********************************************************************
 * @fn b_chown
 * @brief
 * @param[inout]
 * @param[inout]
 * @return
 ********************************************************************** */
word b_chown(word str, word owner)
{
  /* TODO */
  return -1;
}

/* ***********************************************************************
 * @fn b_close
 * @brief Closes the file.
 * @param[in] f Number of file to close.
 * @return 0 or positive on success, negative on failure.
 ********************************************************************** */
word b_close(word f)
{
  /* TODO */
  return -1;
}

/* ***********************************************************************
 * @fn b_creat
 * @brief
 * @param[inout]
 * @param[inout]
 * @return
 ********************************************************************** */
word b_creat(word str, word mode)
{
  /* TODO */
  return -1;
}

/* ***********************************************************************
 * @fn b_ctime
 * @brief
 * @param[inout]
 * @param[inout]
 * @return
 ********************************************************************** */
void b_ctime(word time, word date)
{
  /* TODO */
}

/* ***********************************************************************
 * @fn b_execl
 * @brief
 * @param[inout]
 * @param[inout]
 * @return
 ********************************************************************** */
void b_execl(word str, word arg0)
{
  /* TODO */
}


/* ***********************************************************************
 * @fn b_execv
 * @brief
 * @param[inout]
 * @param[inout]
 * @return
 ********************************************************************** */
void b_execv(word str, word argvec, word count)
{
  /* TODO */
}


/* ***********************************************************************
 * @fn b_exit
 * @brief  Terminate the current process.
 ********************************************************************** */
void b_exit(void)
{
  exit(0);
}


/* ***********************************************************************
 * @fn b_fork
 * @brief
 * @param[inout]
 * @param[inout]
 * @return
 ********************************************************************** */
word b_fork(void)
{
  /* TODO */
  return -1;
}


/* ***********************************************************************
 * @fn b_fstat
 * @brief
 * @param[inout]
 * @param[inout]
 * @return
 ********************************************************************** */
word b_fstat(word f, word status)
{
  /* TODO */
  return -1;
}


/* ***********************************************************************
 * @fn b_getchar
 * @brief Gets next character from stdin.
 * @return Next character or EOF (-1)
 ********************************************************************** */
word b_getchar(void)
{
  word rtn = -1;
  rtn = getchar(); /* use C getchar */
  return rtn;
}
  

/* ***********************************************************************
 * @fn b_getuid
 * @brief Returns current user ID
 * @return Current user ID
 ********************************************************************** */
word b_getuid(void)
{
  return 0;
}


/* ***********************************************************************
 * @fn b_gtty
 * @brief Gets modes of tty.
 * @param[in] f File number to report on.
 * @param[out] ttystat Vector to put modes into.
 * @return  TODO
 ********************************************************************** */
word b_gtty(word f, word ttystat)
{
  /* TODO */
  return -1;
}


/* ***********************************************************************
 * @fn b_lchar
 * @brief Stores character into str.
 * @param[inout] str String to store into.
 * @param[in] i Index to place character.
 * @param[in] ch Character to store into string.
 ********************************************************************** */
void b_lchar(word str, word i, word ch)
{
  word index;
  word wrd;
  index = i >> 1;
  wrd = str[index];
  if(i & 1)  /* Odd chars go on right */
  {
    wrd &= 0xff00;
    wrd |= (ch & 0xff);
  }
  else
  {
    wrd &= 0xff;
    wrd |= (ch & 0xff) << 8;
  }
}


/* ***********************************************************************
 * @fn b_link
 * @brief Creates a link named str2 to file named by str1
 * @param[in] str1 Name of file to link to.
 * @param[in] str2 Name of new link to file.
 * @return 0 or positive on success, negative on failure.
 ********************************************************************** */
word b_link(word str1, word str2)
{
  /* TODO */
  return -1;
}


/* ***********************************************************************
 * @fn b_mkdir
 * @brief
 * @param[in] str Name of directory to make
 * @param[in] mode File mode of new directory.
 * @return
 ********************************************************************** */
word b_mkdir(word str, word mode)
{
  /* TODO */
  return -1;
}


/* ***********************************************************************
 * @fn b_open
 * @brief Opens the named file with given access.
 * @param[in] str Name / path of file to open.
 * @param[in] mode String indicating mode to open e.g. "r", "w", "a"
 * @return File number or -1 on error.
 ********************************************************************** */
word b_open(word str, word mode)
{
  /* TODO */
  return -1;
}


/* ***********************************************************************
 * @fn b_printf
 * @brief Prints to stdout using format string and other parameters.
 * @param[in] format String to print and use for formatting.
 * @param[in] arg1 .. argn Parameters to print with format string.
 * @return
 ********************************************************************** */
void b_printf(word format/* , word argl, ... */)
{
  /* TODO */
}


/* ***********************************************************************
 * @fn b_printn
 * @brief Prints a non-negative number in base (2-10).
 * @param[in] number The number to print.
 * @param[in] base The base to use when printing number.
 ********************************************************************** */
void b_printn(word number, word base)
{
  /* TODO */
  /* From K Thompson's manual */
  word a;
  if(a = number / base) /* assignment, not test for equality */
  {
    b_printn(a,base); /* recursive */
  }
  putchar(number % base + '0');
}


/* ***********************************************************************
 * @fn b_putchar
 * @brief Sends a character to stdout.
 * @param[in] ch Character to send.
 ********************************************************************** */
void b_putchar(word ch)
{
  putchar(ch); /* Use C putchar */
}


/* ***********************************************************************
 * @fn b_read
 * @brief Reads data from input file.
 * @param[in] f File number to read from.
 * @param[out] buffer Buffer to store data to.
 * @param[in] count Number of words to read.
 * @return Number of words actually read, -1 on error.
 ********************************************************************** */
word b_read(word f, word buffer, word count)
{
  word n;
  n = read(f, buffer, count*2);  /* Use C read, C uses bytes not words */
  return n;
}


/* ***********************************************************************
 * @fn b_seek
 * @brief Seeks to specific location in a file.
 * @param[in] f File number to seek.
 * @param[in] offset Location to seek to relative to pointer.
 * @param[in] pointer 0 for start, 1 for current pos, 2 for end of file.
 * @return Negative if error.
 ********************************************************************** */
word b_seek(word f, word offset, word pointer)
{
  word rtn;
  rtn = 0;
  /* TODO */
  return(-1);
}


/* ***********************************************************************
 * @fn b_setuid
 * @brief Sets current user ID.
 * @param[in] id New user ID to set.
 * @return Negative on error.
 ********************************************************************** */
word b_setuid(word id)
{
  /* TODO */
  return(-1);
}


/* ***********************************************************************
 * @fn b_stat
 * @brief Gets inode of file str into vector status.
 * @param[inout]
 * @param[inout]
 * @return
 ********************************************************************** */
word b_stat(word str, word status)
{
  /* TODO */
  return(-1);
}


/* ***********************************************************************
 * @fn
 * @brief
 * @param[inout]
 * @param[inout]
 * @return
 ********************************************************************** */
word b_stty(word f, word ttystat)
{
  /* TODO */
  return(-1);
}


/* ***********************************************************************
 * @fn b_time
 * @brief Gets current system time into vector timev.
 * @param[out] timev Vector to place time in.
 ********************************************************************** */
void b_time(word timev)
{
  /* TODO */
}


/* ***********************************************************************
 * @fn b_unlink
 * @brief
 * @param[inout]
 * @param[inout]
 * @return
 ********************************************************************** */
word b_unlink(word str)
{
  /* TODO */
  return(-1);
}


/* ***********************************************************************
 * @fn b_wait
 * @brief  Suspend current process until a child terminates.
 * @return
 ********************************************************************** */
word b_wait(void)
{
  /* TODO */
  return(-1);
}


/* ***********************************************************************
 * @fn b_write
 * @brief Write a buffer to a file.
 * @param[in] f File number to write to.
 * @param[in] buffer Buffer of words to write.
 * @param[in] count Number of words to write.
 * @return Number of words actually written ???
 ********************************************************************** */
word b_write(word f, word buffer, word count)
{
  word towrite;
  word written;
  towrite = count*2;

  /* TODO */

  return(written);
}



