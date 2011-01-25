#ifndef CS111_OSPSH_H
#define CS111_OSPSH_H

/*
 * UCLA CS 111 - Winter 2011 - Lab 1
 * Header file for Lab 1B - Shell processing
 * This file contains the definitions required for executing commands
 * parsed in part A.
 */

#include "cmdline.h"
#include "makeq.h"

/* Execute the command list. */
int command_line_exec(command_t *);

#endif
