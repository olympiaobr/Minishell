/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:22:41 by olobresh          #+#    #+#             */
/*   Updated: 2024/03/26 11:44:16 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>

typedef enum token_type
{
	T_COMMAND
    T_PIPE,
    T_IN,      // <
    T_OUT,      // >
	T_ENV,
	T_HEREDOC, // <<
    T_ APPEND,  // >>
} token_type;


typedef struct s_token
{
	token_type		type;
	char				*value;
	struct s_token		*next;
}	t_token;

#endif
