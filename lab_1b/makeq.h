/*
 *  makeq.h
 *  lab_1
 *
 *  Created by Mark Vismonte and Timothy Wang on 1/24/11.
 *  Copyright 2011 University of California, Los Angeles. All rights reserved.
 *
 */
#include "cmdline.h"

typedef struct qcommand qcommand_t;

struct qcommand {
    command_t *cmd;
    qcommand_t *next;
    pid_t pid;
};

typedef struct {
    int max_jobs;
    qcommand_t *head;
    qcommand_t *last_run;
    int num_jobs_running;
} makeq_t;

makeq_t * makeq_alloc(void);

//Adds a command to the queue
int add_command(command_t cmd);

//Starts up processes in the queue if we have enough space
void kick_queue(void);

//Reclaims processes after they have finished running, also gets rid of zombie processes
void find_finished_commands(void);

//Runs all the commands in the queue
void wait_queue(void);