/*
 *  makeq.c
 *  lab_1
 *
 *  Created by Mark Vismonte and Timothy Wang on 1/24/11.
 *  Copyright 2011 University of California, Los Angeles. All rights reserved.
 *
 */

#include <stdlib.h>
#include <string.h>
#include "makeq.h"


makeq_t *
makeq_alloc(void)
{
	// Allocate memory for the command
	makeq_t *makeq = (makeq_t *) malloc(sizeof(*makeq));
	if (!makeq)
		return NULL;
	
	// Set all its fields to 0
	memset(makeq, 0, sizeof(*makeq));
    
	return makeq;
}

//Adds a command to the queue
int add_command(command_t *cmd) {
    
}

//Starts up processes in the queue if we have enough space
void kick_queue();

//Reclaims processes after they have finished running, also gets rid of zombie processes
void find_finished_commands();

//Runs all the commands in the queue
void wait_queue();