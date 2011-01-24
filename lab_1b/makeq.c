/*
 *  makeq.c
 *  lab_1
 *
 *  Created by Mark Vismonte and Timothy Wang on 1/24/11.
 *  Copyright 2011 University of California, Los Angeles. All rights reserved.
 *
 */

#include "makeq.h"

//Adds a command to the queue
int add_command(command_t cmd);

//Starts up processes in the queue if we have enough space
void kick_queue();

//Reclaims processes after they have finished running, also gets rid of zombie processes
void find_finished_commands();

//Runs all the commands in the queue
void wait_queue();