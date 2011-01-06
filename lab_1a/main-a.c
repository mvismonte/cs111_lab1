/* 
 * UCLA CS 111 - Fall 2007 - Lab 1
 * Skeleton code for commandline parsing for Lab 1 - Shell processing
 * This file contains skeleton code for parsing input from the commandline.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include "cmdline.h"

/* 
 * Main function for testing commandline parsing.
 */
int
main(int argc, char *argv[])
{
	// String used to store the shell input
	char input_buffer[BUFSIZ];

	// Ignore our arguments
	(void) argc, (void) argv;
	
	while (1) {
		parsestate_t parsestate;
		command_t *cmdlist;
		// Print the prompt, and make sure it gets to the screen
		// right away
		printf("cs111_fall07$ ");
		fflush(stdout);
		
		// Read a string, checking for error or EOF
		if (fgets(input_buffer, BUFSIZ, stdin) == NULL) {
			if (ferror(stdin))
				// This function prints a description of the
				// error, preceded by 'cs111_fall07: '.
				perror("cs111_fall07");
			break;
		}

		parse_init(&parsestate, input_buffer);
		cmdlist = command_line_parse(&parsestate, 0);
		if (!cmdlist)
			fprintf(stderr, "Syntax error\n");

		// Print the command
		command_print(cmdlist, 0);
		command_free(cmdlist);
	}

	printf("\n");
	return 0;
}
