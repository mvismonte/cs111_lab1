/*
 *  tab_completion.c
 *  lab_1
 *
 *  Created by Mark Vismonte & Timothy Wang on 1/26/11.
 *  Copyright 2011 University of California, Los Angeles. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "tab_completion.h"

/*
 * pathcommand_alloc()
 *
 *   Allocates and returns a new blank pathcommand.
 */

pathcommand_t *
pathcommand_alloc(void)
{
	// Allocate memory for the pathcommand
	pathcommand_t *pathcmd = (pathcommand_t *) malloc(sizeof(*pathcmd));
	if (!pathcmd)
		return NULL;
	
	// Set all its fields to 0
	memset(pathcmd, 0, sizeof(*pathcmd));
    
	return pathcmd;
}

/*
 * pathcommand_free(pathcommand_t *)
 * 
 * Frees a pathcommand
 */
void
pathcommand_free(pathcommand_t *pathcmd)
{	
	if (pathcmd->cmd) {
        free(pathcmd->cmd);
    }
    pathcommand_free(pathcmd->left);
    pathcommand_free(pathcmd->right);
	
	free(pathcmd);
}

void add_pathcommand(pathcommand_t *pathcmd) {
    if (!pathcmd)
        return;
    if (!HEAD) {
        HEAD = pathcmd;
    } else {
        add_pathcommand_recur(HEAD, pathcmd);
    }
}

void add_pathcommand_recur(pathcommand_t *current, pathcommand_t *pathcmd) {
    
}



char **
command_completion(char *str, int start, int end) {
    char **matches;
    matches = NULL;
    
    if (start == 0) {
        //do something else
    }
    
    return matches;
}