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
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "cmdline.h"
#include "ospsh.h"

/* command_exec(cmd, pass_pipefd)
 *
 *   Execute the single command specified in the 'cmd' command structure.
 *
 *   The 'pass_pipefd' argument is used for pipes.
 *   On input, '*pass_pipefd' is the file descriptor used to read the
 *   previous command's output.  That is, it's the read end of the previous
 *   pipe.  It equals STDIN_FILENO if there was no previous pipe.
 *   On output, command_exec should set '*pass_pipefd' to the file descriptor
 *   used for reading from THIS command's pipe.
 *   If this command didn't have a pipe -- that is, if cmd->commandop != PIPE
 *   -- then it should set '*pass_pipefd = STDIN_FILENO'.
 *
 *   Returns the process ID of the forked child, or < 0 if some system call
 *   fails.
 *
 *   You must also handle the internal commands "cd" and "exit".
 *   These are special because they must execute in the shell process, rather
 *   than a child.  (Why?)
 *
 *   However, these special commands still have a status!
 *   For example, "cd DIR" should return status 0 if we successfully change
 *   to the DIR directory, and status 1 otherwise.
 *   Thus, "cd /tmp && echo /tmp exists" should print "/tmp exists" to stdout
 *   iff the /tmp directory exists.
 *   Not only this, but redirections should work too!
 *   For example, "cd /tmp > foo" should create an empty file named 'foo';
 *   and "cd /tmp 2> foo" should print any error messages to 'foo'.
 *
 *   How can you return a status, and do redirections, for a command executed
 *   in the parent shell?
 *   Hint: It is easiest if you fork a child ANYWAY!
 *   You should divide functionality between the parent and the child.
 *   Some functions will be executed in each process.
 */
static pid_t
command_exec(command_t *cmd, int *pass_pipefd)
{
	pid_t pid = -1;		// process ID for child
	int pipefd[2];		// file descriptors for this process's pipe

	/* EXERCISE: Complete this function!
	 * We've written some of the skeleton for you, but feel free to
	 * change it.
	 */

	// Create a pipe, if this command is the left-hand side of a pipe.
	// Return -1 if the pipe fails.
    if (cmd->argv[0] == NULL && cmd->subshell == NULL)
        return -1;
    
    if (strcmp(cmd->argv[0], "makeq") == 0) {
        if (cmd->argv[1] != NULL) {
            MAKEQ = makeq_alloc();
            pipe(MAKEQ->pipe);
            //strcpy(MAKEQ->name, cmd->argv[1]);
        } else {
            return -1;
        }
        if (cmd->argv[2] != NULL) {
            MAKEQ->max_jobs = atoi(cmd->argv[2]);
        } else {
            return -1;
        }
    }
    
	if (cmd->controlop == CMD_PIPE) {
		if (pipe(pipefd) == -1)
			return -1;
	}
    
	pid = fork();
    if (pid == -1) {
        return -1; //or error
    }
    
	if (pid == 0) {
        if (strcmp(cmd->argv[0], "makeq") == 0) {
            //int current_jobs = 0;
            FILE *stream = fdopen(MAKEQ->pipe[0], "r");
            char buffer[512];
            fcntl(MAKEQ->pipe[0], F_SETFL, O_NONBLOCK | fcntl(MAKEQ->pipe[0], F_GETFL, 0));
            while (1) {
                //printf("loop\n");
                if (fgets(buffer, 512, stream)) {
                    printf("Added! (%s)", buffer);
                }
                //sleep(1);
            }
        } else {
            int fd;
            dup2(*pass_pipefd, 0);
            
            if (cmd->redirect_filename[0]) {
                fd = open(cmd->redirect_filename[0], O_RDONLY);
                dup2(fd, 0);
                close(fd);
            }
            
            if (cmd->controlop == CMD_PIPE) {
                dup2(pipefd[1], 1);
                close(pipefd[0]);
            } else if (cmd->redirect_filename[1]) {
                *pass_pipefd = STDIN_FILENO;
                fd = open(cmd->redirect_filename[1], O_TRUNC|O_CREAT|O_WRONLY, 0666);
                dup2(fd, 1);
                close(fd);	
            }
            else {
                *pass_pipefd = STDIN_FILENO;
                dup2(STDOUT_FILENO, 1);
            }
            
            if (cmd->redirect_filename[2]) {
                fd = open(cmd->redirect_filename[2], O_CREAT|O_WRONLY, 0666);
                dup2(fd, 2);
                close(fd);
            }
            
            if (cmd->subshell) {
                int exit_status = command_line_exec(cmd->subshell);
                exit(exit_status ? EXIT_FAILURE : EXIT_SUCCESS);
            } else if (strcmp(cmd->argv[0], "cd") == 0) {
                if (cmd->argv[1]) {
                    int fd = open(cmd->argv[1], O_RDONLY);
                    if (fd != -1)
                        close(fd);
                    else {
                        exit(EXIT_FAILURE);
                    }
                    exit(EXIT_SUCCESS);
                }
            } else if (strcmp(cmd->argv[0], "q") == 0) {
                FILE *stream = fdopen(MAKEQ->pipe[1], "a");
                int i;
                for (i = 1; i < MAXTOKENS + 1 && cmd->argv[i] != NULL; i++) {
                    fputs(cmd->argv[i], stream);
                }
                fclose(stream);
                exit(0);
            } else {
                execvp(cmd->argv[0], &cmd->argv[0]);
            }
        }
	} 
    else {
        if (strcmp(cmd->argv[0], "makeq") == 0)
            MAKEQ->pid = pid;
            
        if (*pass_pipefd != STDIN_FILENO) {
            close(*pass_pipefd);
		}
        if (cmd->controlop == CMD_PIPE) {
            *pass_pipefd = pipefd[0];
			close(pipefd[1]);
        } else
            *pass_pipefd = STDIN_FILENO;
        
		if (cmd->argv[0]) {
			if (strcmp(cmd->argv[0], "cd") == 0) {
                //printf("IN CD");
				chdir(cmd->argv[1] ? cmd->argv[1] : getenv("HOME"));
			} else if (strcmp(cmd->argv[0], "exit") == 0) {
				exit(0);
			}
		}
	}
	
	
	
	// Fork the child and execute the command in that child.
	// You will handle all redirections by manipulating file descriptors.
	//
	// This section is fairly long.  It is probably best to implement this
	// part in stages, checking it after each step.  For instance, first
	// implement just the fork and the execute in the child.  This should
	// allow you to execute simple commands like 'ls'.  Then add support
	// for redirections: commands like 'ls > foo' and 'cat < foo'.  Then
	// add parentheses, then pipes, and finally the internal commands
	// 'cd' and 'exit'.
	//
	// In the child, you should:
	//    1. Set up stdout to point to this command's pipe, if necessary.
	//    2. Set up stdin to point to the PREVIOUS command's pipe (that
	//       is, *pass_pipefd), if appropriate.
	//    3. Close some file descriptors.  Hint: Consider the read end
	//       of this process's pipe.
	//    4. Set up redirections.
	//       Hint: For output redirections (stdout and stderr), the 'mode'
	//       argument of open() should be set to 0666.
	//    5. Execute the command.
	//       There are some special cases:
	//       a. Parentheses.  Execute cmd->subshell.  (How?)
	//       b. A null command (no subshell, no arguments).
	//          Exit with status 0.
	//       c. "exit".
	//       d. "cd".
	//
	// In the parent, you should:
	//    1. Close some file descriptors.  Hint: Consider the write end
	//       of this command's pipe, and one other fd as well.
	//    2. Handle the special "exit" and "cd" commands.
	//    3. Set *pass_pipefd as appropriate.
	//
	// "cd" error note:
	// 	- Upon syntax errors: Display the message
	//	  "cd: Syntax error on bad number of arguments"
	// 	- Upon system call errors: Call perror("cd")
	//
	// "cd" Hints:
	//    For the "cd" command, you should change directories AFTER
	//    the fork(), not before it.  Why?
	//    Design some tests with 'bash' that will tell you the answer.
	//    For example, try "cd /tmp ; cd $HOME > foo".  In which directory
	//    does foo appear, /tmp or $HOME?  If you chdir() BEFORE the fork,
	//    in which directory would foo appear, /tmp or $HOME?
	//
	//    EXTRA CREDIT: Our "cd" solution changes the
	//    directory both in the parent process and in the child process.
	//    This introduces a potential race condition.
	//    Explain what that race condition is, and fix it.
	//    Hint: Investigate fchdir().

	/* Your code here. */

	// return the child process ID
	return pid;
}


/* command_line_exec(cmdlist)
 *
 *   Execute the command list.
 *
 *   Execute each individual command with 'command_exec'.
 *   String commands together depending on the 'cmdlist->controlop' operators.
 *   Returns the exit status of the entire command list, which equals the
 *   exit status of the last completed command.
 *
 *   The operators have the following behavior:
 *
 *      CMD_END, CMD_SEMICOLON
 *                        Wait for command to exit.  Proceed to next command
 *                        regardless of status.
 *      CMD_AND           Wait for command to exit.  Proceed to next command
 *                        only if this command exited with status 0.  Otherwise
 *                        exit the whole command line.
 *      CMD_OR            Wait for command to exit.  Proceed to next command
 *                        only if this command exited with status != 0.
 *                        Otherwise exit the whole command line.
 *      CMD_BACKGROUND, CMD_PIPE
 *                        Do not wait for this command to exit.  Pretend it
 *                        had status 0, for the purpose of returning a value
 *                        from command_line_exec.
 */
int
command_line_exec(command_t *cmdlist)
{
	int cmd_status = 0;	    // status of last command executed
	int pipefd = STDIN_FILENO;  // read end of last pipe

	while (cmdlist) {
		int wp_status;	    // Hint: use for waitpid's status argument!
				    // Read the manual page for waitpid() to
				    // see how to get the command's exit
				    // status (cmd_status) from this value.

		// EXERCISE: Fill out this function!
		// If an error occurs in command_exec, feel free to abort().

		pid_t id = command_exec(cmdlist, &pipefd);
		if (id <= 0)
			abort();
		
		switch(cmdlist->controlop)
		{
			case CMD_END:
			case CMD_SEMICOLON:
                if (strcmp(cmdlist->argv[0], "makeq") != 0) {
                    waitpid(id, &wp_status, 0);
                    cmd_status = WEXITSTATUS(wp_status);
                }
				break;
			case CMD_AND:
				waitpid(id, &wp_status, 0);
				if (WEXITSTATUS(wp_status) != 0) {
					cmd_status = WEXITSTATUS(wp_status);
					goto done;
				}
				break;
			case CMD_OR:
				waitpid(id, &wp_status, 0);
				if (WEXITSTATUS(wp_status) == 0) {
					cmd_status = 0; // EXIT_SUCCESS
					goto done;
				}
				break;
			case CMD_BACKGROUND:
			case CMD_PIPE:
				cmd_status = 0;
				break;
		}
		//printf("statul: %d\n", wp_status);
        //printf("status: %d\n", cmd_status);
		//printf("%s(%d): controlop = %d, wp_status = %d, cmd_status = %d\n", 
		//			cmdlist->argv[0], id, cmdlist->controlop, wp_status, cmd_status);
		cmdlist = cmdlist->next;
	}

done:
	//printf("%s: cmd_status = %d\n", cmdlist->argv[0], cmd_status);
	return cmd_status;
}
