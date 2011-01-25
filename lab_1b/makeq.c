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
int add_command(makeq_t *makeq, command_t *cmd) {
    qcommand_t *qcmd = (qcommand_t *) malloc(sizeof(qcommand_t));
    if (!qcmd)
        return 1;
    qcmd->cmd = cmd;
    //pid

    if (makeq->head == NULL) {
        makeq->head = qcmd;
        makeq->next_run = NULL;
    } else {
        qcommand_t *q_itr = makeq->next_run;
        while (q_itr->next != NULL) 
            q_itr = q_itr->next;
        q_itr->next = qcmd;
    }
    return 0; //should be PID
}

//Starts up processes in the queue if we have enough space
void kick_queue(makeq_t *makeq) {
    if (makeq->num_jobs_running < makeq->max_jobs) {
        //pipe kickstart makeq->next_run;
        makeq->next_run = makeq->next_run->next;
        makeq->num_jobs_running++;
    }
    

}

//Reclaims processes after they have finished running, also gets rid of zombie processes
void find_finished_commands(makeq_t *makeq) {
    qcommand_t *q_itr = makeq->head;
    qcommand_t *rem_q;
    qcommand_t *prev;
    while (q_itr != makeq->next_run) {
        if (q_itr->pid == 0) {
            if (q_itr == makeq->head)
                makeq->head = makeq->head->next;
            if (prev != NULL) { //for middle removal
                prev->next = q_itr->next;
            }
            rem_q = q_itr;
            q_itr = q_itr->next;
            free(rem_q);    //free
            //do something with that pid? like clean zomb
        }
        else {
            prev = q_itr;
            q_itr = q_itr->next;
        }

    }

}

//Runs all the commands in the queue
void wait_queue(makeq_t *makeq);
