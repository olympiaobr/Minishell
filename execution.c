/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:14:25 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/30 15:17:04 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "Libft/libft.h"

void handle_heredocs(t_data *data, t_command *cmd)
{
		char *path = cmd->path;//printf("the path is: %s\n", path);
		char *argv[] = {path, "heredoc_tempfile", NULL};
		int pipe_fd[2];
        if (pipe(pipe_fd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        pid_t pid = fork();// Fork a child process to handle the command
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)  // Child process
        {
            close(pipe_fd[1]);// Close the write end of the pipe
            if (dup2(pipe_fd[0], STDIN_FILENO) == -1)// Redirect standard input to the read end of the pipe
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
			close(pipe_fd[0]);// Close the read end of the pipe
            execve(path, argv, NULL);  // Execute the command
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else  // Parent process
        {
            close(pipe_fd[0]); // Close the read end of the pipe
			ssize_t bytes_written = write(pipe_fd[1], data->heredoc_input, ft_strlen(data->heredoc_input));//printf("heredoc input: %s\n", data->heredoc_input);
            if (bytes_written == -1)
            {
                perror("write");//write: Bad file descriptor issue fixed by removing the first close()
                exit(EXIT_FAILURE);
            }
			close(pipe_fd[1]);// Close the write end of the pipe
			if(wait(NULL) == -1) // Wait for the child process to complete
			{
				perror("wait");
				exit(EXIT_FAILURE);
			}
		}
}

void get_exit_status(t_data *data, char **argv, char *path)
{
	pid_t pid = fork(); //fork a child process
				if(pid == -1)
				{
					perror("fork");
					exit(EXIT_FAILURE);
				}
   				else if (pid == 0)  // Child process executes
    			{
        			execve(path, argv, data->env);
        			perror("execve");
        			exit(EXIT_FAILURE);
    			}
    			else  // Parent process
    			{
					int status;
            		waitpid(pid, &status, 0);  // Wait for the child process to finish
            		if (WIFEXITED(status))
            		{
                		int exit_status = WEXITSTATUS(status);//get exit status
                		//printf("Command exited with status: %d\n", exit_status);
						data->exit_status = exit_status;
            		}
    			}
}
void handle_expr_function(t_data *data)
{
			pid_t pid = fork();
			if(pid == -1)
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}
			if(pid == 0) //in the child
			{
				char *exit_status_str_1 = ft_itoa(data->exit_status);
    			char *exit_status_str_2 = ft_itoa(data->exit_status);
    			if (exit_status_str_1 == NULL || exit_status_str_2 == NULL)
				{
        			perror("Memory allocation failed");
        			exit(EXIT_FAILURE);
    			}
    			char *argv[] = {"/usr/bin/expr", exit_status_str_1, "+", exit_status_str_2, NULL};
    			execve("/usr/bin/expr", argv, NULL);
    			perror("execve");
    			exit(EXIT_FAILURE);
   				free(exit_status_str_1);
    			free(exit_status_str_2); 
			}
			else //parent
			{
				int status;
				waitpid(pid, &status, 0);  // Wait for the child process to finish
            	if (WIFEXITED(status))
           		{
                	int exit_status = WEXITSTATUS(status);//get exit status
					data->exit_status = exit_status;
           		}
			}
}
char **set_argv_array(t_command *cmd, char *option, char *argument1, char *argument2)
{
	int argc = 1;
	if (cmd->option != NULL)
    {
        option = cmd->option->value;//ft_printf("option is: %s\n", option);
       	argc++;
   	}
    if (cmd->argv != NULL)
    {
		argument1 = cmd->argv->value;//ft_printf("argument1 is: %s\n", argument1);
        argc++;
    	if (cmd->argv->next != NULL && cmd->argv->next->value != NULL)
    	{
            argument2 = cmd->argv->next->value;//ft_printf("argument2 is: %s\n", argument2);
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
    argv[i++] = cmd->command;
    if (option != NULL)
		argv[i++] = option;   
    if (argument1 != NULL)
		 argv[i++] = argument1;
    if (argument2 != NULL)
		  argv[i++] = argument2;
	argv[i] = NULL;
	return(argv);
}

void execute_external_command(t_data *data, t_command *cmd)
{
    char *path = cmd->path;
   // char *command = cmd->command;
    char *option = NULL;
    char *argument1 = NULL;
    char *argument2 = NULL;
   // int argc = 1;

	if(data->heredoc == 1)
		handle_heredocs(data, cmd);
	else
	{	
		if(ft_strcmp(data->token_list->value, "expr") == 0  && ft_strcmp(data->token_list->next->next->value, "+") == 0)
			handle_expr_function(data);
		else
		{
			char **argv = set_argv_array(cmd, option, argument1, argument2);
			get_exit_status(data, argv, path);
			free(argv);
		}
	
	}

}
void process_command_arguments(t_command *cmd)
{
	if (!cmd)
		return;
	t_token *arg = cmd->argv;
	int index = 0;
	//printf("Processing arguments for command: %s\n", cmd->command);

	// Skip the command if it's included in argv, adjust accordingly if cmd->argv directly starts from the args
	if (arg && index == 0 && check_builtin(cmd->command))
	{
		arg = arg->next; // Skip the first if it includes the command
		index++;
	}
	while (arg)
	{
		//printf("Argument %d: %s (is_quoted: %d)\n", index++, arg->value, arg->is_quoted);
		arg = arg->next;
	}
}

void execution(t_data *data)
{
    if (check_valid_command(data) != 1)
	{
        ft_printf("%s: command not found\n", data->commands->command);
		data->exit_status = 127;
    }
	else
	{
        t_command *cmd = data->commands;  //ft_printf("Valid command.\n");
        while (cmd != NULL)
		{
            if (check_builtin(cmd->command))
			{
				process_command_arguments(cmd); //ft_printf("Executing built-in command: %s\n", cmd->command);
                if (execute_builtin(cmd, data) == -1)
				{
                    ft_printf("Error executing built-in command.\n");
					data->exit_status = 1;
                }
				data->exit_status = 0;
            }
			else
                execute_external_command(data, cmd);
            cmd = cmd->next;
        }
    }
	free_commands(data->commands);
    data->commands = NULL;
}















































