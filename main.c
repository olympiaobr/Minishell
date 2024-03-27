/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:39:02 by jasnguye          #+#    #+#             */
/*   Updated: 2024/03/27 14:39:35 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

 int main(int argc, char *argv[])
{
	(void)argv;
	t_data data;
	if (argc == 1)
	{
		data.user_input = readline("minishell: ");
		ft_printf("%s\n", data.user_input);
		lexing_input(&data);
		t_token *current = data.token_list;
		while(current != NULL)
		{
			ft_printf("Value: %s\n", current->value);
			current = current->next;
		}
		free(data.user_input);
		//free token list
	}
	else
	{
		ft_printf("Non-valid number of arguments!");
	}

}



