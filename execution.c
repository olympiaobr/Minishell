/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:14:25 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/22 17:56:45 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "Libft/libft.h"

void bla(t_data *data, t_command *cmd)
{
		char *path = cmd->path;
		printf("the path is: %s\n", path);
		char *argv[] = {cmd->command, "heredoc_tempfile", NULL};

		int pipe_fd[2];
        if (pipe(pipe_fd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        // Fork a child process to handle the command
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)  // Child process
        {
            // Close the read end of the pipe
            close(pipe_fd[0]);

            // Redirect standard input to the write end of the pipe
            if (dup2(pipe_fd[1], STDIN_FILENO) == -1)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }

            // Close the write end of the pipe
            close(pipe_fd[1]);

            // Execute the command
            execve(path, argv, NULL);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else  // Parent process
        {
            // Close the write end of the pipe
            close(pipe_fd[1]);

            // Write heredoc input to the pipe
			printf("heredoc input: %s\n", data->heredoc_input);
            ssize_t bytes_written = write(pipe_fd[0], data->heredoc_input, ft_strlen(data->heredoc_input));
            if (bytes_written == -1)
            {
                perror("write");//write: Bad file descriptor issue
                exit(EXIT_FAILURE);
            }

            // Close the read end of the pipe
            close(pipe_fd[0]);

            // Wait for the child process to complete
            wait(NULL);
		}
}

void execute_external_command(t_data *data, t_command *cmd)
{
    char *path = cmd->path;
    char *command = cmd->command;
    char *option = NULL;
    char *argument1 = NULL;
    char *argument2 = NULL;
    int argc = 1;

	if(data->heredoc == 1)
	{
		bla(data, cmd);
	}
	else
	{
		 if (cmd->option != NULL)
    	{
        	option = cmd->option->value;
        	ft_printf("option is: %s\n", option);
       	 	argc++;
   		}
    	if (cmd->argv != NULL)
    	{
        	argument1 = cmd->argv->value;
        	ft_printf("argument1 is: %s\n", argument1);
        	argc++;
        	if (cmd->argv->next != NULL && cmd->argv->next->value != NULL)
        	{
            	argument2 = cmd->argv->next->value;
            	ft_printf("argument2 is: %s\n", argument2);
            	argc++;
        	}
    	}

    	char **argv = malloc(sizeof(char *) * (argc + 1));
    	if (!argv)
    	{
        	ft_printf("memory allocation failed\n");
        	exit(EXIT_FAILURE);
    	}

    	int i = 0;
    	argv[i++] = command;
    	if (option != NULL)
    	{
        	argv[i++] = option;
    	}
    	if (argument1 != NULL)
    	{
        	argv[i++] = argument1;
    	}
    	if (argument2 != NULL)
    	{
        	argv[i++] = argument2;
    	}
   	 	argv[i] = NULL;

   		if (fork() == 0)  // Child process
    	{
        	execve(path, argv, data->env);
        	perror("execve");
        	exit(EXIT_FAILURE);
    	}
    	else  // Parent process
    	{
        	wait(NULL);
    	}
		free(argv);
	}
   
}

void execution(t_data *data)
{
    if (check_valid_command(data) != 1)
	{
        ft_printf("Not a valid command.\n");
    }
	else
	{
        ft_printf("Valid command.\n");
        t_command *cmd = data->commands;
        while (cmd != NULL) 
		{
            if (is_builtin(cmd->command))
			{
                ft_printf("Executing built-in command: %s\n", cmd->command);
                if (execute_builtin(cmd, data) == -1)
				{
                    ft_printf("Error executing built-in command.\n");
                }
            }
			else
			{
                execute_external_command(data, cmd);
            }
            cmd = cmd->next;
        }
    }
}


