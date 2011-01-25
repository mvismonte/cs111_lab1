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
    int pipe[2];
    pid_t pid;
};

typedef struct {
    char *name;
    int max_jobs;
    qcommand_t *q;
    qcommand_t *running;
    int num_running;
} makeq_t;

qcommand_t *qcommand_alloc(void);

void qcommand_free(qcommand_t *qcommand);

makeq_t *makeq_alloc(void);

void makeq_free(makeq_t *makeq);

//Adds a command to the queue
int add_command(makeq_t *makeq, command_t *cmd);

//Starts up processes in the queue if we have enough space
void kick_queue(makeq_t *makeq);

//Reclaims processes after they have finished running, also gets rid of zombie processes
void find_finished_commands(makeq_t *makeq);

//Runs all the commands in the queue
void wait_queue(makeq_t *makeq);

#endif