/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:14:25 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/12 21:05:58 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "Libft/libft.h"
void check_error(int fd)
{
	if (fd == -1)
    {
        perror("open output file");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2 for output file");
        close(fd); // Close file descriptor before exiting
        exit(EXIT_FAILURE);
    }
}
void child_process_heredoc(t_data *data, char *path, char **argv, int *pipe_fd)
{
	int fd;
	int flags;

	close(pipe_fd[1]); // Close the write end of the pipe in the child process
    if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
    {
        perror("dup2");
        exit(EXIT_FAILURE);
    }
    close(pipe_fd[0]); // Close the now redundant original read end of the pipe
    if (data->output_file)
    {
        flags = O_WRONLY | O_CREAT;
        if (data->append)
            flags |= O_APPEND;
        else
            flags |= O_TRUNC;
        fd = open(data->output_file, flags, 0644);
		check_error(fd);
        close(fd); // Close file descriptor after dup2
    }
    execve(path, argv, NULL); // Execute the command
    perror("execve");
    exit(EXIT_FAILURE);
}

void parent_process_heredoc(t_data *data, int *pipe_fd)
{
	ssize_t bytes_written;

	close(pipe_fd[0]); // Close the read end of the pipe in the parent process
    if (data->heredoc_input)
    {
		bytes_written = write(pipe_fd[1],
				data->heredoc_input, strlen(data->heredoc_input));
    	if (bytes_written == -1)
    	{
        	perror("write to pipe");
        	exit(EXIT_FAILURE);
    	}
    	if (data->heredoc_input[strlen(data->heredoc_input) - 1] != '\n')
    	{
        	write(pipe_fd[1], "\n", 1);
    	}
    	free(data->heredoc_input);  // Free after writing to the pipe
    	data->heredoc_input = NULL;
    }
    close(pipe_fd[1]); // Close the write end of the pipe after writing
    if (wait(NULL) == -1)
    {
        perror("wait");
        exit(EXIT_FAILURE);
    }
}
void execute_heredoc(t_data *data, char **argv, int *pipe_fd, char *path)
{
	pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(argv); // Free argv before exiting
        close(pipe_fd[0]); // Close pipe ends before exiting
        close(pipe_fd[1]);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)  // Child process
    {
		child_process_heredoc(data, path, argv, pipe_fd);
    }
    else  // Parent process
    {
		parent_process_heredoc(data, pipe_fd);
    }
    free(argv); // Free argv in the parent after all operations
}
void handle_heredocs(t_data *data, t_command *cmd)
{
    char **argv;
	int pipe_fd[2];
    char *path;

	path = cmd->path; //  printf("the path is: %s\n", path);
    argv = malloc(2 * sizeof(char *));
    if (argv == NULL)
    {
        perror("Failed to allocate memory for argv");
        exit(EXIT_FAILURE);
    }
    argv[0] = path;
    argv[1] = NULL;
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        free(argv); 
        exit(EXIT_FAILURE);
    }
	execute_heredoc(data, argv, pipe_fd, path);
} 

/* 
void handle_heredocs(t_data *data, t_command *cmd)
{
    char **argv;
    char *path = cmd->path;
    printf("the path is: %s\n", path);
    argv = malloc(2 * sizeof(char *));
    if (argv == NULL)
    {
        perror("Failed to allocate memory for argv");
        exit(EXIT_FAILURE);
    }
    argv[0] = path;
    argv[1] = NULL;

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        free(argv); // Free argv before exiting
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(argv); // Free argv before exiting
        close(pipe_fd[0]); // Close pipe ends before exiting
        close(pipe_fd[1]);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)  // Child process
    {
        close(pipe_fd[1]); // Close the write end of the pipe in the child process
        if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipe_fd[0]); // Close the now redundant original read end of the pipe
        if (data->output_file)
        {
            int flags = O_WRONLY | O_CREAT;
            if (data->append)
                flags |= O_APPEND;
            else
                flags |= O_TRUNC;
            int fd = open(data->output_file, flags, 0644);
            if (fd == -1)
            {
                perror("open output file");
                exit(EXIT_FAILURE);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2 for output file");
                close(fd); // Close file descriptor before exiting
                exit(EXIT_FAILURE);
            }
            close(fd); // Close file descriptor after dup2
        }
        execve(path, argv, NULL); // Execute the command
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else  // Parent process
    {
        close(pipe_fd[0]); // Close the read end of the pipe in the parent process
        if (data->heredoc_input)
        {
            ssize_t bytes_written = write(pipe_fd[1], data->heredoc_input, strlen(data->heredoc_input));
            if (bytes_written == -1)
            {
                perror("write to pipe");
                exit(EXIT_FAILURE);
            }
            if (data->heredoc_input[strlen(data->heredoc_input) - 1] != '\n')
            {
                write(pipe_fd[1], "\n", 1);
            }
            free(data->heredoc_input);  // Free after writing to the pipe
            data->heredoc_input = NULL;
        }
        close(pipe_fd[1]); // Close the write end of the pipe after writing
        if (wait(NULL) == -1)
        {
            perror("wait");
            exit(EXIT_FAILURE);
        }
    }
    free(argv); // Free argv in the parent after all operations
} 

 */

void parent_process_expr(t_data *data, pid_t pid)
{
	int status;
	waitpid(pid, &status, 0);  // Wait for the child process to finish
    if (WIFEXITED(status))
    {
        int exit_status = WEXITSTATUS(status);//get exit status
		data->exit_status = exit_status;
    }
}
void child_process_expr(t_data *data)
{
	char *argv[5];
	char *exit_status_str_1;
    char *exit_status_str_2;

	exit_status_str_1 = ft_itoa(data->exit_status);
	exit_status_str_2 = ft_itoa(data->exit_status);
    if (exit_status_str_1 == NULL || exit_status_str_2 == NULL)
	{
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
	argv[0] = "/usr/bin/expr";
    argv[1] = exit_status_str_1;
    argv[2] = "+";
    argv[3] = exit_status_str_2;
    argv[4] = NULL;
    execve("/usr/bin/expr", argv, NULL);
    perror("execve");
    exit(EXIT_FAILURE);
   	free(exit_status_str_1);
    free(exit_status_str_2);
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
				child_process_expr(data);
			}
			else //parent
				parent_process_expr(data, pid);
}

int validate_command(t_command *cmd) 
{
    if (!cmd->path || access(cmd->path, X_OK) != 0) {
        perror("Invalid command path or command is not executable");
        return 0;
    }
    return 1;
}

int validate_io_channels(int *io) 
{
    if (io[0] < 0 || io[1] < 0) {
        fprintf(stderr, "Error: Invalid IO channels determined.\n");
        return 0;
    }
    return 1;
}

int count_arguments(t_command *cmd) 
{
    int argc = 1; // Count the command itself
    if (cmd->option != NULL) {
        argc++;
    }
    t_token *arg = cmd->argv;
    while (arg) {
        argc++;
        arg = arg->next;
    }
    return argc;
}

char **create_argv(t_command *cmd) 
{
    int argc = count_arguments(cmd);
    char **argv = malloc(sizeof(char *) * (argc + 1));
    if (!argv) {
        perror("Memory allocation failed for argv");
        return NULL;
    }

    int i = 0;
    argv[i++] = cmd->command;
    if (cmd->option != NULL) {
        argv[i++] = cmd->option->value;
    }

    t_token *arg = cmd->argv;
    while (arg) {
        argv[i++] = arg->value;
        arg = arg->next;
    }
    argv[i] = NULL;

    return argv;
}



void setup_io_channels(int *io) 
{
    if (io[0] != STDIN_FILENO) {
        dup2(io[0], STDIN_FILENO);
        close(io[0]);
    }
    if (io[1] != STDOUT_FILENO) {
        dup2(io[1], STDOUT_FILENO);
        close(io[1]);
    }
}

void execute_command(t_command *cmd, char **argv, char **env) 
{
    execve(cmd->path, argv, env);
    perror("Execve failed");
    exit(EXIT_FAILURE);
}

void handle_parent_process(t_data *data, pid_t pid) 
{
    int status = 0;
    int ret;
    while ((ret = waitpid(pid, &status, 0)) == -1) {
    }
    if (ret == -1) {
        perror("waitpid failed");
    } else if (WIFEXITED(status)) {
        data->exit_status = WEXITSTATUS(status);
    }
}

void execute_external_command(t_data *data, t_command *cmd) 
{
    if (!validate_command(cmd)) 
        return;
    int io[2];
    determine_io_channels(data, cmd->command_index, io);
    if (!validate_io_channels(io)) 
        return;
    char **argv = create_argv(cmd);
    if (!argv) 
        return;
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        free(argv);
        return;
    }
    if (pid == 0)  // Child process
	{
        setup_io_channels(io);
        execute_command(cmd, argv, data->env);
    } 
	else // Parent process
	{
        handle_parent_process(data, pid);
        free(argv);
    }
}






void execute_simple_command(t_data *data, t_command *cmd)
{
	int exit_status;

    if (check_builtin(cmd->command))
	{
		exit_status = execute_builtin(cmd, data);//ft_printf("Debug: Executing built-in command: %s\n", cmd->command);
        data->exit_status = exit_status;
        return;
    }
    if (data->heredoc == 1)
        handle_heredocs(data, cmd);
	else if (ft_strcmp(data->token_list->value, "expr") == 0 && data->token_list->next &&
               ft_strcmp(data->token_list->next->next->value, "+") == 0)
       	handle_expr_function(data);
	else
        execute_external_command(data, cmd);//ft_printf("Debug: After executing command, std_input_fd = %d, std_output_fd = %d\n", data->std_input_fd, data->std_output_fd);
	if (data->std_input_fd != STDIN_FILENO)   //Close and reset the input and output file descriptors if they were redirected
    {
        close(data->std_input_fd);//ft_printf("Debug: Closing std_input_fd = %d\n", data->std_input_fd);
        data->std_input_fd = STDIN_FILENO;
    }
    if (data->std_output_fd != STDOUT_FILENO)
    {
        close(data->std_output_fd);//ft_printf("Debug: Closing std_output_fd = %d\n", data->std_output_fd);
        data->std_output_fd = STDOUT_FILENO;
    }
}


void execution(t_data *data)
{
    if (!data || !data->commands || !data->commands->command)
        return;
    if (check_valid_command(data) != 1)
	{
        ft_printf("%s: command not found\n", data->commands->command);
        data->exit_status = 127;
        return;
    }
    count_commands(data);  //ft_printf("Debug: Number of commands counted = %d\n", data->count_cmd);
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
        execute_simple_command(data, data->commands);  //ft_printf("Debug: Executing simple command...\n");
    free_commands(data->commands);
    data->commands = NULL;
	free_pipesfd(data);
}
