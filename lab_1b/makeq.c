/*
 *  makeq.c
 *  lab_1
 *
 *  Created by Mark Vismonte and Timothy Wang on 1/24/11.
 *  Copyright 2011 University of California, Los Angeles. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "makeq.h"
#include <sys/wait.h>
//temporary add

qcommand_t *qcommand_alloc(void) {
    // Allocate memory for the command
	qcommand_t *qcommand = (qcommand_t *) malloc(sizeof(*qcommand));
	if (!qcommand)
		return NULL;
	
	// Set all its fields to 0
	memset(qcommand, 0, sizeof(*qcommand));
    
	return qcommand;
}

void
qcommand_free(qcommand_t *qcommand) {
    close(qcommand->pipe[0]);
    close(qcommand->pipe[1]);
    if (qcommand) {
        free(qcommand);
    }
}

//void qcommand_free(qcommand_t *qcommand);

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
    if (q) {
        if (q->name)
            free(q->name);
        free(q);
    }
}

//Adds a command to the queue
int add_command(qcommand_t *to_q) {
    if (to_q == NULL || MKQ == NULL) // return if either are null
        return 1;
        
    if (MKQ->q == NULL) {// if the q is null
        to_q->next = NULL;
        MKQ->q = to_q; // make to_q the head
    } else { //else
        qcommand_t *itr; //add it to the end of the q
        for (itr = MKQ->q; itr != NULL; itr = itr->next)
            if (itr->next == NULL)
                itr->next= to_q;
    }
    kick_queue();
    return 0;
}

//Starts up processes in the queue if we have enough space
void kick_queue() {
    if (!MKQ || MKQ->q == NULL)
        return;
    //printf("kickQ 1 jobs: %d\n", MKQ->num_running);
    while (MKQ->num_running < MKQ->max_jobs && MKQ->q != NULL) {
        (MKQ->num_running)++;
        qcommand_t *next = MKQ->q;
        MKQ->q = next->next;
        //qcommand_t *head; //unused
        if (MKQ->running == NULL) {
            next->next = NULL;
            MKQ->running = next;
        } else {
            next->next = MKQ->running;
            MKQ->running = next;
        }
        sleep(1);
        write(next->pipe[1], "X", 1);//start running process
    }
    //printf("kickQ 2 jobs: %d\n", MKQ->num_running);
}

//Reclaims processes after they have finished running, also gets rid of zombie processes
void find_finished_commands() {
    if (!MKQ || MKQ->num_running == 0)
        return;
    //printf("Number of Jobs running: %d\n", MKQ->num_running);
    qcommand_t *head, *trail;
    for (head = MKQ->running, trail = NULL; head != NULL; ) {
        //printf("Looking up pid: %d\n", head->pid);
        if (waitpid(head->pid, NULL, WNOHANG)) {
            (MKQ->num_running)--;
            //printf("Jobs running: %d\n", MKQ->num_running);
            if (head == MKQ->running) {
                MKQ->running = head->next;
                //qcommand_free(head);
                head = MKQ->running;
                trail = NULL;
                if (head == NULL)   //redundant?
                    break;
            } else {
                trail->next = head->next;
                qcommand_free(head);
                head = trail->next;
            }
        } else {
            trail = head;
            head = head->next;
            //printf("Job not finished running\n");
        }
    }
    //printf("Number of Jobs running: %d\n", MKQ->num_running);
    /*qcommand_t *new_runnable = NULL;
    qcommand_t *current = NULL;
    qcommand_t *itr;
    for (itr = MKQ->running; itr != NULL; ) {
        if (!waitpid(itr->pid, NULL, WNOHANG)) {
            if (new_runnable == NULL) {
                new_runnable = itr;
                current = new_runnable;
            } else {
                current->next = itr;
                current = current->next;
                current->next = NULL;
            }
            itr = itr->next;
        } else {
            MKQ->num_running--;
            qcommand_t *temp = itr->next;
            //qcommand_free(itr);
            itr = temp;
        }
    }
    MKQ->running = new_runnable;*/
}


//Runs all the commands in the queue
void wait_queue() {
    //printf("wait_queue\n");
    //implement some type of blocking mechanism that runs all commands
    while (MKQ->running != NULL && MKQ->q != NULL) {
        qcommand_t *last = MKQ->running;
        while (last->next)
            last = last->next;
        waitpid(last->pid, NULL, 0);
        kick_queue();
    }
    
}
