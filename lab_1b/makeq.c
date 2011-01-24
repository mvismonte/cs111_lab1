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

void
makeq_free(makeq_t *q) {
    free(q->name);
    free(q);
}

//Adds a command to the queue
int add_command(makeq_t *makeq, command_t *cmd) {
    /*if (makeq == NULL || cmd == NULL)
        return -2;
    
    qcommand_t *next = (qcommand_t *) malloc(*next);
    if (!next)
        return -1;
    
    next->cmd = cmd;
    next->pid = -1; //set pid to -1 when a process has not been run yet
    if (*/
    return 0;
}

//Starts up processes in the queue if we have enough space
void kick_queue();

//Reclaims processes after they have finished running, also gets rid of zombie processes
void find_finished_commands();

//Runs all the commands in the queue
void wait_queue();