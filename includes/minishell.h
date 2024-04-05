/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:22:41 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/02 16:55:47 by jasnguye         ###   ########.fr       */
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

typedef enum token_type
{
	T_COMMAND,  //general command
	T_PIPE,
	T_IN,  // <
	T_OUT,  // >
	T_ENV,
	T_HEREDOC,  // <<
	T_APPEND,  // >>
	T_ARGUMENT,  //command arguments
	T_FILE,  //filename for redirections
}					token_type;

typedef struct s_token
{
	token_type		type;
	char			*value;
	struct s_token	*next;
	/*bool			single_quotes;*/
	 int is_quoted;
}					t_token;

typedef struct data_all
{
    t_token *token_list;
    char    *user_input;
    t_token *current_token;
    char    *input_file;    // For < redirection
    char    *output_file;   // For > or >> redirection
    int     append;         // Flag for append mode (>>)
    int     heredoc;        // Flag for heredoc (<<)
}   t_data;

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
t_token	*init_token(token_type type);
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

//free functions
void	free_tokens(t_data *data);

#endif
