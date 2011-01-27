/*
 * UCLA CS 111 - Winter 2011 - Lab 1
 * Skeleton code for Lab 1 - Shell processing
 * This file contains skeleton code for executing commands parsed in part A.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "cmdline.h"
#include "ospsh.h"
#include "tab_completion.h"

/* EXERCISE: Make sure you free memory used by the command_t structures
 * when it is no longer needed.
 * EXERCISE:  Make sure you clean up ALL zombie processes, including those
 * that ran in the background!
 * There are several ways to do this, and several places to put the code.
 * Check out the SIGCHLD signal and the waitpid() system call for hints.
 * HINT:
 *   You DO NOT need to clean up zombies as soon as they are created.
 *   It is OK to clean up zombies as little as once per command.
 *   That is, it is OK if a zombie hangs around until the next time the user
 *   types a command.  But it is NOT OK for zombies to hang around forever.
 */
#define PROMPT_SIZE 256

int regular_main();
int quiet_main();
void sig_child(int intr);


/*
 * Main function for shell.
 * Need to implement this using readline (man readline)
 */
int
main(int argc, char *argv[])
{
    signal(SIGCHLD, sig_child);
    
    initialize_path_tree();
    print_tree();

	// Check for '-q' option: be quiet -- print no prompts
	if (argc > 1 && strcmp(argv[1], "-q") == 0)
        return quiet_main();
    else
        return regular_main();
}

int regular_main() {
    char *buf;
    char prompt[PROMPT_SIZE];
	int r = 0;
    
    rl_attempted_completion_function = (CPPFunction *)command_completion;
    command_completion("l", 0, 2);
        
    while (!feof(stdin)) {
		parsestate_t parsestate;
		command_t *cmdlist;
        prompt[0] = 0;//clear the prompt
        buf = NULL;
        
		// Print the prompt
        sprintf(prompt, "%c[%d;%dmcs111_winter11%c[%dm",27,1,32,27,0);
        if (r != 0)
            sprintf(prompt + strlen(prompt), "%c[%d;%dm(exit=%d)%c[%dm",27,1,31,r, 27,0);
        sprintf(prompt + strlen(prompt), "$ ");
        fflush(stdout);
        
        buf = readline(prompt);
        if (!buf) {
            fprintf(stderr, "Exiting...\n");
            exit(1);
        }
        if (strcmp(buf,"") == 0)
            continue;
        
        // build the command list
        parse_init(&parsestate, buf);
        
        cmdlist = command_line_parse(&parsestate, 0);
        if (!cmdlist) {
            printf("Syntax error\n");
            continue;
        }
        
        // print the command list
        command_print(cmdlist, 0);
        // why do we need to do this?
        fflush(stdout);
        
        // and run it!
        if (cmdlist)
            r = command_line_exec(cmdlist);
        command_free(cmdlist);
		
		while (waitpid(-1, NULL, WNOHANG) > 0)
            /* Try again */;
        
        if (buf) {
            add_history(buf);
            free(buf);
        }
        
	}
   
    return r;
}

int quiet_main() {
    char input[BUFSIZ];
	int r = 0;
    
    while (!feof(stdin)) {
		parsestate_t parsestate;
		command_t *cmdlist;
        
		// Read a string, checking for error or EOF
		if (fgets(input, BUFSIZ, stdin) == NULL) {
			if (ferror(stdin)) {
                if (errno != EINTR) {
                    // This function prints a description of the
                    // error, preceded by 'cs111_winter11: '.
                    perror("cs111_winter11");
                }
                
            }
            break;
		} //need to figure out how signals can be used
        
        // build the command list
        parse_init(&parsestate, input);
        
        cmdlist = command_line_parse(&parsestate, 0);
        if (!cmdlist) {
            printf("Syntax error\n");
            continue;
        }
        
        // and run it!
        if (cmdlist)
            r = command_line_exec(cmdlist);
        command_free(cmdlist);
		
		while (waitpid(-1, NULL, WNOHANG) > 0)
            /* Try again */;
        
	}
    return r;
}



void sig_child(int intr) {
    //sleep(1);
    //printf("Child death\n");
    find_finished_commands();
    kick_queue();
}
