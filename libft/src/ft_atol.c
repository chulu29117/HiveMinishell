/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:54:27 by clu               #+#    #+#             */
/*   Updated: 2025/02/11 17:04:32 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	overflow_check(const char *str, int sign)
{
	long	res;

	res = 0;
	while (*str && ft_isdigit(*str))
	{
		if (res > (LONG_MAX / 10))
		{
			if (sign == -1)
				return (LONG_MIN);
			return (LONG_MAX);
		}
		if (res == (LONG_MAX / 10) && (*str - '0') > (LONG_MAX % 10))
		{
			if (sign == -1)
				return (LONG_MIN);
			return (LONG_MAX);
		}
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res * sign);
}

long	ft_atol(const char *str)
{
	int		sign;

	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	return (overflow_check(str, sign));
}
