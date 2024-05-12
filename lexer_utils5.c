/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:11:11 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/12 16:11:13 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

t_token *get_last_token(t_token *token_list) {
    t_token *last_token = token_list;
    while (last_token && last_token->next) {
        last_token = last_token->next;
    }
    return last_token;
}


void update_expectations(t_data *data, int *expect_command, token_type *type) {
    t_token *last_token = get_last_token(data->token_list);
    if (last_token) {
        if (last_token->type == T_HEREDOC) {
            *expect_command = 0;
            *type = T_DELIMITER;
        } else if (last_token->type == T_PIPE || last_token->type == T_OUT ||
                   last_token->type == T_IN || last_token->type == T_APPEND) {
            *expect_command = 1;
            *type = T_COMMAND;
        } else {
            *type = T_ARGUMENT;
        }
    }
}

void handle_operator(t_data *data, char *str, size_t *idx, int *expect_command, token_type *type) {
    tokenize_operator(data, str, idx);
    update_expectations(data, expect_command, type);
}


int process_character(t_data *data, char *str, size_t *idx, token_type type) {
    if (whitespace_chars(str[*idx])) {
        (*idx)++;
        return 0;
    }
    if (shell_operators(str[*idx])) {
        tokenize_operator(data, str, idx);  // Assuming tokenize_operator does not return any value
        return 0;  // Assuming no errors are handled here since tokenize_operator is void
    } else {
        return tokenize_word(data, str, idx, type);  // token_type is passed here
    }
}


int handle_normal_character(t_data *data, char *str, size_t *idx, int *expect_command, token_type *type) {
    if (*expect_command) {
        *type = T_COMMAND;
        *expect_command = 0;
    } else {
        *type = T_ARGUMENT;
    }
    return process_character(data, str, idx, *type);
}


void process_input(t_data *data, char *str) {
    size_t idx = 0;
    int expect_command = 1;
    token_type type = T_COMMAND;

    while (str[idx]) {
        if (whitespace_chars(str[idx])) {
            idx++;
            continue;
        }
        if (shell_operators(str[idx])) {
            handle_operator(data, str, &idx, &expect_command, &type);
        } else {
            if (handle_normal_character(data, str, &idx, &expect_command, &type) != 0) {
                ft_printf("Failed to tokenize input at index %zu\n", idx);
                free_tokens(data);
                break;
            }
        }
    }
}
