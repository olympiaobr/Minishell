/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 15:59:11 by jasnguye          #+#    #+#             */
/*   Updated: 2024/03/27 11:04:25 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// to be improved 
#include "Libft/libft.h"
#include "includes/minishell.h"
#define MAX_TOKEN_LENGTH 500

// const char *find_end(const char *input, char *delimiter)
// {
// 	while(*input != '\0')
// 	{
// 		char *ptr = delimiter;
// 		while(*ptr != '\0')
// 		{
// 			if(*input == *ptr)
// 			{
// 				return(input);
// 			}
// 			ptr++;
// 		}
// 		input++;
// 	}
// 	return(NULL);
// }

// void tokenize_input(const char *input)
// {
// 	char *delimiter = " \t\n|<>;&";
// 	const char *token_start;
// 	const char *token_end;

// 	while (*input != '\0')//like input[i]
// 	{
// 		while(*input != '\0' && ft_strchr(input, 32) != NULL && ft_strchr(input, 9) != NULL && ft_strchr(input, 10) != NULL) // skip space, newline and tab
// 		{
// 			input++; //like i++;
// 		}
// 		token_start = input;
// 		token_end = find_end(input, delimiter);
// 	}
// 	if(token_end == NULL)
// 	{
// 		token_end = input + ft_strlen(input);
// 	}
// 	if (token_start != token_end) 
// 	{
//         char token[MAX_TOKEN_LENGTH];
//         strncpy(token, token_start, token_end - token_start);
//         token[token_end - token_start] = '\0';
//         printf("Token: %s\n", token);
//     }
// //    while (*token_end != '\0' && strchr(" \t\n", *token_end) != NULL) 
// // 	{
// //             token_end++;
// //     }
	
// 	input = token_end;
// } 



// /* 
// int main(void)
// {
// 	const char input[] = "ls -l ";
// 	tokenize_input(input);
// }
//  */
