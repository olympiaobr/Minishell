/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 14:25:39 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/12 14:25:40 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void process_characters(char *str, size_t *idx, int *in_quote, char *quote_char)
{
    while (str[*idx])
	{
        if (str[*idx] == '\\' && *in_quote && *quote_char == '\"' &&
            (str[*idx + 1] == '$' || str[*idx + 1] == '\"' || str[*idx + 1] == '\\'))
			{
            (*idx) += 2;
            continue;
        }
        quote_status(str[*idx], in_quote, quote_char);
        if (!*in_quote && (whitespace_chars(str[*idx]) || shell_operators(str[*idx])))
		{
            break;
        }
        (*idx)++;
    }
}

char *determine_word(char *str, size_t start_idx, size_t length, char quote_char)
{
    char *word;

    if (quote_char && str[start_idx] == quote_char && str[start_idx + length - 1] == quote_char)
	{
        start_idx++;
        length -= 2;
    }
    word = ft_substr(str, start_idx, length);
    if (quote_char && str[start_idx - 1] != quote_char && str[start_idx + length] != quote_char)
	{
        char *temp = edge_case(word);
        free(word);
        return temp;
    }
    return word;
}


int set_quoted_flag(char quote_char)
{
    if (quote_char == '\'')
	{
        return 1;
    }
	else if (quote_char == '\"')
	{
        return 2;
    }
    return 0;
}
void adjust_indices(size_t *start_idx, size_t *length, char quote_char, char *str, size_t *idx, int in_quote) {
    if (quote_char && str[*start_idx] == quote_char && str[*idx - 1] == quote_char && !in_quote && *length >= 2) {
        (*start_idx)++; 
        (*length) -= 2;  // Reduce length to remove both quotes
    }
}

int handle_token_finalization(t_data *data, char *word, char quote_char, token_type expected_type) {
    int is_quoted;
    is_quoted = set_quoted_flag(quote_char);
    printf("Tokenizing: %s, is_quoted: %d\n", word, is_quoted);
    if (create_and_append_token(&data->token_list, word, expected_type, is_quoted) != 0) {
        free(word);
        return -1;
    }
    free(word);
    return 0;
}

int tokenize_word(t_data *data, char *str, size_t *idx, token_type expected_type) {
    size_t start_idx = *idx;
    int in_quote = 0;
    char quote_char = '\0';
    size_t length;
    char *word;

    process_characters(str, idx, &in_quote, &quote_char);
    length = *idx - start_idx;
    adjust_indices(&start_idx, &length, quote_char, str, idx, in_quote);

    word = determine_word(str, start_idx, length, quote_char);
    if (word) {
        return handle_token_finalization(data, word, quote_char, expected_type);
    }
    return -1;
}

token_type set_token_type(int *expect_command, int *expect_delimiter) {
    if (*expect_command) {
        *expect_command = 0;
        return T_COMMAND;
    } else if (*expect_delimiter) {
        *expect_delimiter = 0;
        return T_DELIMITER;
    }
    return T_ARGUMENT;
}


void update_expectations(t_data *data, int *expect_command, int *expect_delimiter) {
    t_token *last_token = data->token_list;
    while (last_token && last_token->next) {
        last_token = last_token->next;
    }
    if (last_token) {
        if (last_token->type == T_HEREDOC) {
            *expect_delimiter = 1;
            *expect_command = 0;
        } else if (last_token->type == T_PIPE || last_token->type == T_OUT ||
                   last_token->type == T_IN || last_token->type == T_APPEND) {
            *expect_command = 1;
            *expect_delimiter = 0;
        }
    }
}

int process_character(t_data *data, char *str, size_t *idx, int *expect_command, int *expect_delimiter) {
    token_type type;
    if (whitespace_chars(str[*idx])) {
        (*idx)++;
        return 0;
    }
    if (shell_operators(str[*idx])) {
        tokenize_operator(data, str, idx);
        update_expectations(data, expect_command, expect_delimiter);
    } else {
        type = set_token_type(expect_command, expect_delimiter);
        if (tokenize_word(data, str, idx, type) != 0) {
            return -1;
        }
    }
    return 0;
}


void process_input(t_data *data, char *str) {
    size_t idx = 0;
    int expect_command = 1;
    int expect_delimiter = 0;

    while (str[idx]) {
        if (process_character(data, str, &idx, &expect_command, &expect_delimiter) != 0) {
            ft_printf("Failed to tokenize input at index %zu\n", idx);
            free_tokens(data);
            break;
        }
    }
}

