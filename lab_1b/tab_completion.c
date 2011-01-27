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
#include <sys/types.h>
#include <sys/dir.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "tab_completion.h"

#include <dirent.h>

pathcommand_t *HEAD;

//private functions
void add_pathcommand(char *cmd);
void add_pathcommand_recur(pathcommand_t *current, pathcommand_t *pathcmd);
char **find_matches(char *str);
void print_tree_recur(pathcommand_t *cur);

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
    if (pathcmd == NULL)
        return;
	if (pathcmd->cmd) {
        free(pathcmd->cmd);
    }
    pathcommand_free(pathcmd->left);
    pathcommand_free(pathcmd->right);
	
	free(pathcmd);
}

void add_pathcommand(char *cmd) {
    size_t len = strlen(cmd);
    if (len == 0)
        return;
    pathcommand_t * new_pathcmd = pathcommand_alloc();
    new_pathcmd->cmd = cmd;
    new_pathcmd->len = len;
    if (!HEAD) {
        HEAD = new_pathcmd;
    } else {
        add_pathcommand_recur(HEAD, new_pathcmd);
    }
}

void add_pathcommand_recur(pathcommand_t *current, pathcommand_t *to_add) { //should guarentee that neither are null
    size_t small_len = current->len < to_add->len ? current->len : to_add->len;
    if (strncmp(current->cmd, to_add->cmd, small_len) < 0) {
        if (current->left == NULL) {
            current->left = to_add;
        } else {
            add_pathcommand_recur(current->left, to_add);
        }
    } else {
        if (current->right == NULL) {
            current->right = to_add;
        } else {
            add_pathcommand_recur(current->right, to_add);
        }
    }
}

void
initialize_path_tree(void) {
    char *path = getenv("PATH");

    // Add shell commands
    add_pathcommand("cd");
    add_pathcommand("exit");

    /* getting each dir : */
    char *dir = (char *) malloc(sizeof(*path));
    memset(dir, 0, sizeof(*path));
    unsigned int path_s = 0;
    unsigned int i;
    while (path[path_s] != '\0') {
        i = 0;
        while (path[path_s] != ':' && path[path_s] != '\0') {
            dir[i] = path[path_s];
            ++path_s;
            ++i;
        }
        dir[i] = '\0';
        ++path_s;
        DIR *curr_dir = opendir(dir);
        while (1) {
            struct dirent *dir_item = readdir(curr_dir);
            if (dir_item == NULL)
                break;
            /* Convert dirent to char * ? */
            char *command = dir_item->d_name;  //?
            add_pathcommand(command);
        }

    }
}

void print_tree() {
    print_tree_recur(HEAD);
}

void print_tree_recur(pathcommand_t *cur) {
    if (NULL)
        return;
    print_tree_recur(cur->left);
    printf("%s\n", cur->cmd);
    print_tree_recur(cur->right);
}

char **
find_matches(char *str) {
    return NULL;
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
