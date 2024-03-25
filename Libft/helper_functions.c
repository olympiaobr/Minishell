/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:00:10 by jasnguye          #+#    #+#             */
/*   Updated: 2024/03/15 14:06:28 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_char(int c)
{
	return (write (1, &c, 1)); 
}

int	print_string(char *str)
{
	int	count;

	count = 0;
	if (str == NULL)
	{
		print_string("(null)");
		return (6);
	}
	while (*str != '\0')
	{
		print_char((int)*str);
		str++;
		count++;
	}
	return (count);
}

int	print_digit(long nbr, int base, int boolean)
{
	int		count;
	char	*symbols;

	if (boolean == 1)
		symbols = "0123456789ABCDEF";
	else
		symbols = "0123456789abcdef";
	if (nbr < 0)
	{
		write (1, "-", 1);
		return (print_digit(-nbr, base, boolean) + 1);
	}
	else if (nbr < base)
	{
		return (print_char(symbols[nbr]));
	}
	else
	{
		count = print_digit(nbr / base, base, boolean);
		return (count + print_digit(nbr % base, base, boolean));
	}
}

int	print_pointer(unsigned long long int nbr)
{
	int		count;
	char	*symbols;

	symbols = "0123456789abcdef";
	if (nbr < 16)
	{
		return (print_char(symbols[nbr]));
	}
	else
	{
		count = print_pointer(nbr / 16);
		return (count + print_pointer(nbr % 16));
	}
}

int	print_ox(va_list arguments)
{
	int						count; 
	unsigned long long int	number;

	number = va_arg(arguments, unsigned long long int);
	if (number == 0)
	{
		return (write (1, "(nil)", 5));
	}
	count = 0;
	write (1, "0x", 2);
	count += print_pointer(number);
	return (count + 2);
}
