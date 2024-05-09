/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:14:25 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/06 16:08:02 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "Libft/libft.h"

void handle_heredocs(t_data *data, t_command *cmd)
{
		char *path = cmd->path;
		printf("the path is: %s\n", path);
		char *argv[] = {path, "heredoc_tempfile", NULL};

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
            // Close the write end of the pipe
            close(pipe_fd[1]);

            // Redirect standard input to the read end of the pipe
            if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }

            // Close the read end of the pipe
			close(pipe_fd[0]);
            // Execute the command
            execve(path, argv, NULL);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else  // Parent process
        {
            // Close the read end of the pipe
            close(pipe_fd[0]);

            // Write heredoc input to the pipe
			printf("heredoc input: %s\n", data->heredoc_input);
			ssize_t bytes_written =write(pipe_fd[1], data->heredoc_input, ft_strlen(data->heredoc_input));
            if (bytes_written == -1)
            {
                perror("write");//write: Bad file descriptor issue fixed by removing the first close()
                exit(EXIT_FAILURE);
            }
            // Close the write end of the pipe
			close(pipe_fd[1]);

            // Wait for the child process to complete
			if(wait(NULL) == -1)
			{
				perror("wait");
				exit(EXIT_FAILURE);
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


void execute_external_command(t_data *data, t_command *cmd)
{
        if (!cmd->path || access(cmd->path, X_OK) != 0)
		{
            perror("Invalid command path or command is not executable");
            return;
        }
        int io[2];
        determine_io_channels(data, cmd->command_index, io);

        if (io[0] < 0 || io[1] < 0)
        {
        fprintf(stderr, "Error: Invalid IO channels determined.\n");
        return;
        }
        int argc = 1;
        if (cmd->option != NULL)
		{
            argc++;
        }
        t_token *arg = cmd->argv;
        while (arg)
		{
            argc++;
            arg = arg->next;
        }

        char **argv = malloc(sizeof(char *) * (argc + 1));
        if (!argv)
		{
            perror("Memory allocation failed for argv");
            return;
        }

        argv[0] = cmd->command;
        int i = 1;
        if (cmd->option != NULL)
		{
            argv[i++] = cmd->option->value;
        }
        arg = cmd->argv;
    	while (arg)
		{
        	argv[i++] = arg->value;
        	arg = arg->next;
    	}
    	argv[i] = NULL;

        pid_t pid = fork();
        if (pid == -1)
		{
            perror("Fork failed");
            free(argv);
            return;
        }
        if (pid == 0)
		{ // Child process
            if (io[0] != STDIN_FILENO)
			{
                dup2(io[0], STDIN_FILENO);
                close(io[0]);
            }
            if (io[1] != STDOUT_FILENO)
			{
                dup2(io[1], STDOUT_FILENO);
                close(io[1]);
            }
            execve(cmd->path, argv, data->env);
            perror("Execve failed");
            exit(EXIT_FAILURE);
        }
        else
        {  // Parent process
            int status = 0;
            int ret;

            while ((ret = waitpid(pid, &status, 0)) == -1)
            {
            }
            if (ret == -1)
            {
             perror("waitpid failed");
            }
            else if (WIFEXITED(status)) {
            data->exit_status = WEXITSTATUS(status);
        }
        free(argv);
    }
}

void execute_simple_command(t_data *data, t_command *cmd)
{
	int exit_status;

    if (check_builtin(cmd->command))
	{
		//ft_printf("Debug: Executing built-in command: %s\n", cmd->command);
		exit_status = execute_builtin(cmd, data);
        data->exit_status = exit_status;
        return;
    }
    if (data->heredoc == 1)
	{
        handle_heredocs(data, cmd);
    }
	else if (ft_strcmp(data->token_list->value, "expr") == 0 &&
               ft_strcmp(data->token_list->next->next->value, "+") == 0)
				{
       				 handle_expr_function(data);
    			}
	else
	{
        execute_external_command(data, cmd);
    }
    ft_printf("Debug: After executing command, std_input_fd = %d, std_output_fd = %d\n", data->std_input_fd, data->std_output_fd);

    // Close and reset the input and output file descriptors if they were redirected
    if (data->std_input_fd != STDIN_FILENO)
    {
        ft_printf("Debug: Closing std_input_fd = %d\n", data->std_input_fd);
        close(data->std_input_fd);
        data->std_input_fd = STDIN_FILENO;
    }
    if (data->std_output_fd != STDOUT_FILENO)
    {
        ft_printf("Debug: Closing std_output_fd = %d\n", data->std_output_fd);
        close(data->std_output_fd);
        data->std_output_fd = STDOUT_FILENO;
    }
}


void execution(t_data *data)
{
    if (check_valid_command(data) != 1)
	{
        ft_printf("%s: command not found\n", data->commands->command);
        data->exit_status = 127;
        return;
    }
    count_commands(data);
    ft_printf("Debug: Number of commands counted = %d\n", data->count_cmd);
    if (operators_setup(data) != 0)
	{
        ft_printf("Failed to setup redirections.\n");
        return;
    }
    if (data->count_cmd > 1)
	{
        ft_printf("Debug: Executing pipeline...\n");
        create_pipes(data);
        execute_pipeline(data, data->commands);
    }
	else
	{
        //ft_printf("Debug: Executing simple command...\n");
        execute_simple_command(data, data->commands);
    }
    free_commands(data->commands);
    data->commands = NULL;
	if(data->pipesfd != NULL)
	{
		int i = 0;
		while(i < data->count_cmd - 1)
		{
			free(data->pipesfd[i]);
			i++;
		}
		free(data->pipesfd);
	}
}

/*
void process_command_arguments(t_command *cmd)
{
	if (!cmd)
		return;
	t_token *arg = cmd->argv;
	int index = 0;
	printf("Processing arguments for command: %s\n", cmd->command);

	// Skip the command if it's included in argv, adjust accordingly if cmd->argv directly starts from the args
	if (arg && index == 0 && check_builtin(cmd->command))
	{
		arg = arg->next; // Skip the first if it includes the command
		index++;
	}
	while (arg)
	{
		printf("Argument %d: %s (is_quoted: %d)\n", index++, arg->value, arg->is_quoted);
		arg = arg->next;
	}
}
*/
