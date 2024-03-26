/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:22:41 by olobresh          #+#    #+#             */
/*   Updated: 2024/03/26 16:01:14 by jasnguye         ###   ########.fr       */
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
	T_COMMAND,
<<<<<<< HEAD
    T_PIPE,
    T_IN,      // <
    T_OUT,      // >
	T_ENV,
	T_HEREDOC, // <<
    T_APPEND // >>
} token_type;

=======
	T_PIPE,
	T_IN,  // <
	T_OUT, // >
	T_ENV,
	T_HEREDOC, // <<
	T_APPEND, // >>
}					token_type;
>>>>>>> b96abfddbb949a41926889d2bb4bd65e6cdf8b01

typedef struct s_token
{
	token_type		type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct data_all
{
	t_token	*token_list;
}	t_data;



#endif
