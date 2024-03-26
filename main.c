/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:39:02 by jasnguye          #+#    #+#             */
/*   Updated: 2024/03/26 17:21:03 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"


 int main(int argc, char *argv[])
{
	
	t_data data;
	if (argc == 1)
	{
		data.user_input = readline("minishell: ");
		ft_printf("%s\n", data.user_input);
		lexing_input(data);
		free(data.user_input);
	}
	else
	{
		ft_printf("Non-valid number of arguments!");
	}

}



