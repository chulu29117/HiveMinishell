/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:56:52 by clu               #+#    #+#             */
/*   Updated: 2025/01/15 16:42:25 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digits(int n);
static void	ft_putnbr_itoa(char *str, int n, int digits);

char	*ft_itoa(int n)
{
	char	*str;
	int		digits;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	digits = count_digits(n);
	str = (char *)malloc(sizeof(char) * (digits + 1));
	if (!str)
		return (NULL);
	ft_putnbr_itoa(str, n, digits);
	str[digits] = '\0';
	return (str);
}

static int	count_digits(int n)
{
	int	digits;

	digits = 0;
	if (n <= 0)
		digits++;
	while (n != 0)
	{
		n = n / 10;
		digits++;
	}
	return (digits);
}

static void	ft_putnbr_itoa(char *str, int n, int digits)
{
	int	sign;
	int	i;

	sign = 1;
	if (n < 0)
	{
		sign = -1;
		n = -n;
	}
	i = digits - 1;
	while (i >= 0)
	{
		str[i] = (n % 10) + '0';
		n = n / 10;
		i--;
	}
	if (sign == -1)
		str[0] = '-';
}
