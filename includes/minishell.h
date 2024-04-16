/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:22:41 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/15 15:10:23 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>

typedef enum token_type
{
	T_COMMAND,  //general command
	T_PIPE,
	T_IN,  // <
	T_OUT,  // >
	T_HEREDOC,  // <<
	T_APPEND,  // >>
	T_ARGUMENT,  //command arguments
	T_ENV,
	T_FILE
}					token_type;

typedef struct s_token
{
	token_type		type;
	char			*value;
	struct s_token	*next;
	/*bool			single_quotes;*/
	 int is_quoted;
}					t_token;

typedef struct s_command
{
	char	*command; //command name like ls, cd,..
	t_token	*argv;
	t_token *option;

	struct s_command *next; //array of command arguments
	int		argc;     //number of command arguments
	//bool built-in?
	char *path;
	char **argv_array;
}	t_command;

typedef struct data_all
{
	t_token	*token_list;
	t_command *command_list;
	char	*user_input;
	t_token *current_token;
	char	*std_input;
	char	*std_output;
    char    *input_file;    // For < redirection
    char    *output_file;   // For > or >> redirection
	int      std_input_fd;   // File descriptor for input redirection
    int      std_output_fd;  // File descriptor for output redirection
    int     append;         // Flag for append mode (>>)
    int     heredoc;        // Flag for heredoc (<<)
	char **path_dirs;  // stores parsed PATH directories
    // char *path;        // stores duplicated PATH env var
	char **env;
	t_command *commands;
	int *create_new_command;
	int exit_status;
}	t_data;

//lexing functions
void lexing_input(t_data *data);
int	whitespace_chars(char c);
void quote_status(char c, int *in_quote, char *quote_char);
int	create_and_append_token(t_token **token_list, char *input, token_type type, int is_quoted);
token_type	determine_type(char *operator);
void tokenize_operator(t_data *data, char *str, size_t *idx);
void tokenize_word(t_data *data, char *str, size_t *idx, token_type expected_type);
// void tokenize_rest(t_data *data, char *str, size_t *idx);
void process_input(t_data *data, char *str);
t_token	*allocate_token(token_type type, char *val, int is_quoted);
void	append_token(t_token **token_list, t_token *new_token);
 int	shell_operators(int c);
 char	*extract_next_word(char *str);
t_token *init_token(token_type type, char *value, int is_quoted);
char	*trim_value(const char *val);

int input_check(const char *input);
int check_special_chars(const char *input);
int error_operators(const char *input);
int wrong_operators(const char *command);
int redirection_error(const char *cmd);
int quote_error(const char *input);
int validate_input(t_data *data);
int	validate_operator_sequence(const char **line);
void count_q(char c, int *sq, int *dq);

//expansion functions
void expansion(t_data *data);
void check_quotes(t_data *data);

//path
char *cust_getenv(const char *name, t_data *data);
int init_env(t_data *data, char **env);
void prepare_environment(t_data *data);

//init
void initialize_shell_components(t_data *data);
t_data *init_data(char **envp);


//parsing functions
int parser(t_data *data);
void tokenize_path(const char *input);
int set_command(t_command *cmd_struct, t_token *current_token);
int add_token_to_list(t_token **token_list, t_token *token);
int add_option_to_command(t_command *cmd, t_token *token);
t_command *init_command(char *command);
t_command *create_command(t_data *data, t_token *token);
int link_arg_to_command(t_command *last_command, t_token *token);
int process_commands(t_data *data, t_token *token, t_command **current_cmd);
//int setup_redirection(t_data *data, t_token *token, int oflag);
int setup_redirection(t_data *data, t_token *token, char *filename, int oflag);
int apply_redirection(t_data *data, t_token *token);
int parser(t_data *data);
int check_valid_command(t_data *data);

//heredoc functions
void check_for_heredoc(t_data *data);


//added helper functions
int ft_strcmp(char *s1, char *s2);
char *ft_strcpy(char *dest, const char *src);
char *ft_strcat(char *dest, char *src);
size_t ft_strspn(const char *str, const char *delim);
size_t ft_strcspn(const char *str, const char *reject);
char *custom_strtok(char *str, const char *delim);

//execution functions
void execution(t_data *data/* , t_command *cmd */);
int is_builtin(const char *command);


//free functions
void	free_tokens(t_data *data);
void free_commands(t_command *commands);
void free_all(t_data *data);

void ft_error(char *err);

#endif
