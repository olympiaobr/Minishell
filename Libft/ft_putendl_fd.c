/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 12:27:46 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 18:14:55 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// modified for builtins
int ft_putendl_fd(char *s, int fd)
{
    size_t len = 0;
    char *buff;

	if (!s)
		return (-1);
    while (s[len] != '\0')
        len++;
    buff = malloc(len + 2);
	if(!buff)
        return (-1);
    ft_memcpy(buff, s, len);
    buff[len] = '\n';
    buff[len + 1] = '\0';
    int result = write(fd, buff, len + 1);
    free(buff);

    return (result);
}
/*
void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
	write(fd, "\n", 1);
}


int main(void)
{
	int file_descriptor = 1;
	char *string = "hello hello";
	ft_putendl_fd(string, file_descriptor);
} */
