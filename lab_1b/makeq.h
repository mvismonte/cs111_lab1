/*
 *  makeq.h
 *  lab_1
 *
 *  Created by Mark Vismonte and Timothy Wang on 1/24/11.
 *  Copyright 2011 University of California, Los Angeles. All rights reserved.
 *
 */
#ifndef MAKEQ_H
#define MAKEQ_H
#include "cmdline.h"

typedef struct qcommand qcommand_t;

struct qcommand {
    command_t *cmd;
    qcommand_t *next;
    pid_t pid;
};

typedef struct {
    int max_jobs;
    int pipe[2];
    char *name;
    qcommand_t *head;
    qcommand_t *last_ran;
    pid_t pid;
} makeq_t;

makeq_t *MAKEQ;

qcommand_t *qcommand_alloc(void);

void qcommand_free(qcommand_t *qcommand);

makeq_t *makeq_alloc(void);

void makeq_free(makeq_t *makeq);

void kill_makeq_process(void);

//Adds a command to the queue
int add_command(makeq_t *makeq, command_t *cmd);

//Starts up processes in the queue if we have enough space
void kick_queue(void);

//Reclaims processes after they have finished running, also gets rid of zombie processes
void find_finished_commands(void);

//Runs all the commands in the queue
void wait_queue(void);

#endif