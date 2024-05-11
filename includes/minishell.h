/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:22:41 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/11 19:03:42 by jasnguye         ###   ########.fr       */
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
# include <signal.h>


extern volatile sig_atomic_t heredoc_interrupted_flag;
extern volatile sig_atomic_t heredoc_mode;
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
	T_FILE,
	T_DELIMITER// for heredoc
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
	int command_index;
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
	int original_stdout; // Store the original STDOUT file descriptor
    int original_stdin;  // Store the original STDIN file descriptor
	int      std_input_fd;   // File descriptor for input redirection
    int      std_output_fd;  // File descriptor for output redirection
    int     append;         // Flag for append mode (>>)
    int     heredoc;        // Flag for heredoc (<<)
	char **path_dirs;  // stores parsed PATH directories
    // char *path;        // stores duplicated PATH env var
	char **env;
	char *heredoc_input;
	t_command *commands;
	int *create_new_command;
	int exit_status;
	int count_cmd;
	int **pipesfd;
	int max_env_size;
	int output_file_present;
}	t_data;

//lexing functions
void lexing_input(t_data *data);
int	whitespace_chars(char c);
void quote_status(char c, int *in_quote, char *quote_char);
int	create_and_append_token(t_token **token_list, char *input, token_type type, int is_quoted);
token_type	determine_type(char *operator);
void tokenize_operator(t_data *data, char *str, size_t *idx);
int tokenize_word(t_data *data, char *str, size_t *idx, token_type expected_type);
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
int setup_redirection(t_data *data, t_token *token, char *filename);
int apply_redirection(t_data *data, t_token *token);
int parser(t_data *data);
int check_valid_command(t_data *data);

//heredoc functions
void check_for_heredoc(t_data *data);
void bla(t_data *data, t_command *cmd);

//added helper functions
int ft_strcmp(const char *s1, const char *s2);
char	*ft_strncpy(char *dest, const char *src, unsigned int n);
char *ft_strcpy(char *dest, const char *src);
char *ft_strcat(char *dest, const char *src);
size_t ft_strspn(const char *str, const char *delim);
size_t ft_strcspn(const char *str, const char *reject);
char *custom_strtok(char *str, const char *delim);

//execution functions
void execution(t_data *data/* , t_command *cmd */);
int check_builtin(const char *command);
int execute_builtin(t_command *cmd, t_data *data);
int cd_cmd(t_data *data, t_command *cmd);
const char *parse_cd_target(t_data *data, t_command *cmd);
int pwd_cmd(void);
int env_cmd(t_data *data);
int echo_cmd(t_command *cmd);
int n_option(const char *arg);
int print_arg(t_token *current_arg, int *first, int newline);
int exit_cmd(t_data *data, t_command *cmd);
int export_cmd(t_data *data, t_command *cmd);
int unset_cmd(t_data *data, t_command *cmd);
void remove_var(t_data *data, const char *var_name);
const char *handle_cd_arguments(t_data *data, t_command *cmd);
int set_env_var(t_data *data, const char *name, const char *value);
int add_new_env(t_data *data, char *new_val, int index);
void display_all_env_vars(char **env);
void append_slash(char **directory);
void handle_export_operation(t_data *data, t_token *arg);
int update_existing_env(t_data *data, char *new_val, int i);
char *get_env_var(char **envp, const char *name);
int valid_identifier(const char *name);
void execute_external_command(t_data *data, t_command *cmd);
void determine_io_channels(t_data *data, int cmd_index, int io[2]);
void execute_pipeline(t_data *data, t_command *cmd);


int operators_setup(t_data *data);
int create_pipes(t_data *data);
int count_commands(t_data *data);
//signal functions
void catch_sigint(int sig);
void handle_sigquit(int signum);
void setup_interactive_signals(void);
void setup_noninteractive_signals(void);
void signal_setup(int mode);
void heredoc_signals();
//free functions
void free_tokens(t_data *data);
void free_commands(t_command *commands);
void free_all(t_data *data);


void ft_error(char *err);
void display_commands(t_data *data);
#endif
