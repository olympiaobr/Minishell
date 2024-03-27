/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:22:41 by olobresh          #+#    #+#             */
/*   Updated: 2024/03/27 13:47:55 by jasnguye         ###   ########.fr       */
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
	T_OUT, // >
	T_ENV,
	T_HEREDOC, // <<
	T_APPEND, // >>
	T_ARGUMENT, //command arguments
	T_FILE, //filename for redirections
}					token_type;

typedef struct s_token
{
	token_type		type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct data_all
{
	t_token	*token_list;
	char	*user_input;
}	t_data;

//lexing functions
void lexing_input(t_data *data);
int	whitespace_chars(char c);
void	quote_status(char c, int *in_single_q, int *in_double_q);
int	create_and_append_token(t_token **token_list, char *input, token_type type);
token_type	determine_type(char *operator);
void tokenize_operator(t_data *data, char *str, size_t *idx);
void tokenize_word(t_data *data, char *str, size_t *idx);
void tokenize_rest(t_data *data, char *str, size_t *idx);
void process_input(t_data *data, char *str);
t_token	*allocate_token(token_type type, char *val);
void	append_token(t_token **token_list, t_token *new_token);
 int	shell_operators(int c);
 char	*extract_next_word(char *str);
t_token	*init_token(token_type type);
#endif
