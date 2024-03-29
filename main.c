/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:39:02 by jasnguye          #+#    #+#             */
/*   Updated: 2024/03/29 12:41:47 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

 int main(int argc, char *argv[])
{
	(void)argv;
    t_data data;

	data.token_list = NULL;
    if (argc == 1)
	{
        data.user_input = readline("minishell: ");
        //ft_printf("%s\n", data.user_input);
        lexing_input(&data);
		expansion(&data);

        t_token *current = data.token_list;
        while (current != NULL)
		{
            ft_printf("Value: %s Type: ", current->value);
            if (current->type == T_COMMAND) {
                ft_printf("T_COMMAND");
            } else if (current->type == T_PIPE) {
                ft_printf("T_PIPE");
            } else if (current->type == T_IN) {
                ft_printf("T_IN");
            } else if (current->type == T_OUT) {
                ft_printf("T_OUT");
            } else if (current->type == T_ENV) {
                ft_printf("T_ENV");
            } else if (current->type == T_HEREDOC) {
                ft_printf("T_HEREDOC");
            } else if (current->type == T_APPEND) {
                ft_printf("T_APPEND");
            } else if (current->type == T_ARGUMENT) {
                ft_printf("T_ARGUMENT");
            } else if (current->type == T_FILE) {
                ft_printf("T_FILE");
            } else {
                ft_printf("Unknown Type");
            }
            ft_printf("\n");
            current = current->next;
        }
		free_tokens(&data);
    }
	else
	{
        ft_printf("Non-valid number of arguments!\n");
    }
    return 0;
}



