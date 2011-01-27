/*
 *  tab_completion.h
 *  lab_1
 *
 *  Created by Mark Vismonte & Timothy Wang on 1/26/11.
 *  Copyright 2011 University of California, Los Angeles. All rights reserved.
 *
 */
#ifndef TAB_COMPLETION_H
#define TAB_COMPLETION_H

//node for our binary tree
typedef struct pathcommand pathcommand_t;

struct pathcommand {
    char *cmd;
    size_t len;
    pathcommand_t *left;
    pathcommand_t *right;
};

pathcommand_t *pathcommand_alloc(void);

void pathcommand_free(pathcommand_t *pathcmd);

char **
command_completion(char *str, int start, int end);

void initialize_path_tree(void);


#endif