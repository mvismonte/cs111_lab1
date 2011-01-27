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
#include <readline/readline.h>
#include <readline/history.h>
#include "tab_completion.h"


char **
command_completion(char *str, int start, int end) {
    char **matches = NULL;
    
    if (start == 0) {
        matches = rl_completion_matches(str, (CPFunction *) command_matcher);
    }
    
    return NULL;
}

char*
command_matcher(char *str, int state) {
    char *path = getenv("PATH");
    printf("\npath: %s\n", path);
    return NULL;
}