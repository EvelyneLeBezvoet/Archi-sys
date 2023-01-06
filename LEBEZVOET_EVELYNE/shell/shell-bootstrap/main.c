#include<string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

#include "global.h"
// This is the file that you should work on.

// declaration
int execute (struct cmd *cmd);
// name of the program, to be printed in several places
#define NAME "myshell"

// Some helpful functions
void CtrlCHandler()
{
	kill(SIGINT,getpid());
}

void errmsg (char *msg)
{
	fprintf(stderr,"error: %s\n",msg);
}

// apply_redirects() should modify the file descriptors for standard
// input/output/error (0/1/2) of the current process to the files
// whose names are given in cmd->input/output/error.
// append is like output but the file should be extended rather
// than overwritten.

FILE* apply_redirects (struct cmd *cmd)
{
	if (cmd->input || cmd->output || cmd->append || cmd->error)
	{
		if (cmd->input)
		{
			if ((strcmp(cmd->input,"\\dev\\null") != 0))
			{
				return freopen(cmd->input,"r",stdin);
			}
		}
		else if (cmd->output)
		{
			if ((strcmp(cmd->output,"\\dev\\null") != 0))
			{
				return freopen(cmd->output,"w",stdout);
			}
		}
		else if (cmd->append)
		{
			if ((strcmp(cmd->append,"\\dev\\null") != 0))
			{
				return freopen(cmd->append,"a",stdout);
			}
		}
		else if (cmd->error)
		{
			if ((strcmp(cmd->error,"\\dev\\null") != 0))
			{
				return freopen(cmd->error,"w",stderr);
			}
		}
	}
	return NULL;
}

// The function execute() takes a command parsed at the command line.
// The structure of the command is explained in output.c.
// Returns the exit code of the command in question.

int execute (struct cmd *cmd)
{
	switch (cmd->type)
	{
	    case C_PLAIN: 
		{
			if ((strcmp(cmd->args[0],"cd") == 0))
			{
				if (chdir(cmd->args[1]) == -1)
					{
						if (cmd->error)
						{
							if (fork()) 
							{
								wait(NULL);
							}

							else 
							{
								FILE *fp = apply_redirects(cmd);
								errmsg("Cannot find the file");
								fclose(fp);
								exit(-1);
							}
						}
						else
						{
							errmsg("Cannot find the file");
						}
						return -1;
					}
			}
			else
			{	
				if (fork()) 
				{
					wait(NULL);
				}

				else 
				{
					FILE *fp = apply_redirects(cmd);
					if ((strcmp(cmd->args[0],"false") == 0))
					{
						exit(-1);
					}
					else
					{
						if (execvp(cmd->args[0],cmd->args) == -1)
						{
							errmsg("The execution of the command failed");
							exit(-1);
						}
					}
					fclose(fp);
					exit(0);
				}
			}
			return 0;
		}
		
	
	    case C_SEQ:
		{
			execute(cmd->left);
			execute(cmd->right);
			return 0;
		}
	    case C_AND:
		{
			int n = execute(cmd->left);
			if (n != -1)
			{
				execute(cmd->right);
			}
			return 0;
		}
			
	    case C_OR:
		{
			int n = execute(cmd->left);
			if (n == -1)
			{
				execute(cmd->right);
			}
			return 0;
		}

	    case C_PIPE: //out de la première dans le in de la deuxième
	    case C_VOID: 
		{
			if (fork()) 
				{
					wait(NULL);
				}

				else 
				{
					FILE *fp = apply_redirects(cmd);
					execute(cmd->left);
					fclose(fp);
					exit(0);
				}
		}
		return -1;
	}

	// Just to satisfy the compiler
	errmsg("This cannot happen!");
	return -1;
}




int main (int argc, char **argv)
{
	signal(SIGINT, CtrlCHandler);
	char *prompt = malloc(strlen(NAME)+3);
	printf("welcome to %s!\n", NAME);
	sprintf(prompt,"%s> ", NAME);
	while (1)
	{
		char *line = readline(prompt);
		if (!line) break;	// user pressed Ctrl+D; quit shell
		if (!*line) continue;	// empty line


		add_history (line);	// add line to history

		struct cmd *cmd = parser(line);
		if (!cmd) continue;	// some parse error occurred; ignore
		output(cmd,0);	// activate this for debugging
		execute(cmd);
	}
	printf("goodbye!\n");
	return 0;
}
