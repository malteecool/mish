#ifndef MISH_H
#define MISH_H
/**
 * @defgroup      mish mish
 * @brief         Simple shellprogram to handle command-line arguments.
 *
 *                The shell handles execution of commands and programs,
 *                pipes betweed multiple programs and redirections of stdio.
 *
 * @{
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include "execute.h"
#include "parser.h"
#include "sighant.h"

#define SIZE 1024

/**
  Global variables to handle
*/

int pid_list[SIZE];
int active_pid;

/**
 * @brief         Prints mish.
 *
 */
void print_dir(void);

/**
 * @brief         Handles execution of commands if no pipe is stated.
 *
 *                The execution is of external commands and is forked
 *                to a new childprocess. The parentprocess holds till
 *                childprocess finishes.
 *
 * @param commands Commands to be executed.
 * @see parser
 * @return        0 on success, -1 on failure.
 */
int execNoPipe(command commands);

/**
 * @brief         Handles exection with pipes.
 *
 *                The program handles multiple pipes with redirection available.
 *                For each command a new childprocess i created.
 *
 * @param commands Commands to be executed.
 * @return        0 on success, -1 on failure.
 */
int execWPipe(command commands[], int pipes);

/**
 * @brief         Handles internal commands.
 *
 *                The internal commands are simple verions of
 *                "cd" and "echo". These commands can too be redireced.
 *
 * @param commands Commands to be executed.
 * @return        0 on success, -0 on failure.
 */
int execOwn(command commands);

/**
 * @brief         Function to print given commands.
 *
 * @param com     Command to print.
 */

void print_command(command com);

/**
 * @brief         Funtion to handle output and redirection of echo command.
 *
 * @param commands Commands and string to be printed by echo.
 * @return        0 on success, -1 on failure.
 */
int echocmd(command commands);

/**@}*/
#endif
