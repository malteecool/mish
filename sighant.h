#ifndef SIGHANT_H
#define SIGHANT_H
/**
 * @defgroup      sighant sighant
 * @brief         Captures signals from mish and handles interuption
 *                of processes.
 *
 * @{
 */
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "mish.h"

/**
 * @brief         Funtion to handle signals.
 *
 *                When captured kills all active processes with
 *                SIGINT.
 *
 * @param sig     Signal to be sent to processes (SIGINT)
 */
void signal_handler(int sig);
/**@}*/
#endif
