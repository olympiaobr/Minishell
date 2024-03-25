/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 12:31:12 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/29 10:30:13 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	int_len(int n)
{
	int	count;

	count = 0; 
	if (n == 0)
	{
		count = 1;
	}
	while (n != 0)
	{
		n /= 10; 
		count++; 
	}
	return (count); 
}

static void	fill_string(char *string, int n, int len, int is_negative)
{
	int	i;

	i = len - 1;
	while (i >= is_negative) 
	{
		string[i--] = (n % 10) + '0';
		n /= 10;
	}
	if (is_negative)
		string[0] = '-';
}

char	*ft_itoa(int n)
{
	int		len;
	int		is_negative;
	char	*string;

	len = int_len(n);
	if (n == -2147483648) 
		return (ft_strdup("-2147483648"));
	if (n < 0) 
		len++; 
	string = (char *)malloc((len + 1) * sizeof(char));
	if (string == NULL) 
		return (NULL);
	string[len] = '\0';
	is_negative = 0;
	if (n < 0) 
	{
		is_negative = 1;
		n = -n;
	}
	fill_string(string, n, len, is_negative);
	return (string);
}
/* int main (void)
 {
    int n = -0;
    char *result = ft_itoa(n); 
    printf ("%s\n", result); 
    free(result); 
 } */