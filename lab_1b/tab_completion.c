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
pathcommand_t *find_first_matching(pathcommand_t *cur, char *str, size_t len);
char **find_matches(pathcommand_t *cur, char *str, size_t len, char** ret, int *index, int *size);
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
    int cmp  = strcmp(current->cmd, to_add->cmd);
    
    if (cmp == 0) { //then don't add it
        pathcommand_free(to_add);
    } else if (cmp > 0) {
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
    add_pathcommand("exit");
    add_pathcommand("cd");
    

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
        if (!curr_dir)
            continue;
        while (1) {
            struct dirent *dir_item = readdir(curr_dir);
            if (dir_item == NULL)
                break;
            /* continue when we see . and .. */
            if (strcmp(dir_item->d_name, ".") == 0)
                continue;
            if (strcmp(dir_item->d_name, "..") == 0)
                continue;
            /* Convert dirent to char * ? */
            char *command = strdup(dir_item->d_name);  //?
            add_pathcommand(command);
        }

    }
}

void print_tree() {
    print_tree_recur(HEAD);
}

void print_tree_recur(pathcommand_t *cur) {
    if (cur == NULL)
        return;
    print_tree_recur(cur->left);
    printf("%s\n", cur->cmd);
    print_tree_recur(cur->right);
}

pathcommand_t *find_first_matching(pathcommand_t *cur, char *str, size_t len) {
    if (cur == NULL)
        return NULL;
    int cmp = strncmp(cur->cmd, str, len);
    if (cmp == 0) {
        return cur;
    } else if (cmp > 0) {
        return find_first_matching(cur->left, str, len);
    } else {
        return find_first_matching(cur->right, str, len);
    }
}

char **
find_matches(pathcommand_t *cur, char *str, size_t len, char **ret, int *index, int *size) {
    if (cur == NULL)
        return ret;
    int cmp = strncmp(cur->cmd, str, len);
    if (cmp >= 0) {
        ret = find_matches(cur->left, str, len, ret, index, size);
    }
    
    if (cmp == 0) {
        
        
        if (*index + 1 >= *size) {
            *size *= 2;
            ret = realloc(ret, (*size) * sizeof(char *));
        }
        ret[*index] = strdup(cur->cmd);
        (*index)++;
        
        
    }
    if (cmp <= 0) {
        ret = find_matches(cur->right, str, len, ret, index, size);
    }
    return ret;
}

char **
command_completion(char *str, int start, int end) {
    //fprintf(stderr, "\n%s: %d, %d\n", str, start, end);
    char **matches = NULL;
    int index = 0, size = 1024;
    int i;
    
    //if (start == 0) {
        //pathcommand_t *first = find_first_matching(HEAD, str, strlen(str));
        //do something else
        matches = malloc(sizeof(char *) * size);
        matches = find_matches(HEAD, str, strlen(str), matches, &index, &size);
        matches[index] = NULL;
    //}
    /*if (matches != NULL) {
        printf("\nMatches==>\n");
        for (i = 0; i < size; i++)
            if (matches[i] != NULL)
                printf("%s\n", matches[i]);
            else {
                break;
            }
        printf("=========\n");
    }
    
    fprintf(stderr, "finished\n");*/
    return matches;
}
