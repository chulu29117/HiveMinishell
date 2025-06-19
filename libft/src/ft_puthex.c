/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:50:19 by clu               #+#    #+#             */
/*   Updated: 2025/01/27 21:30:27 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_puthex(unsigned long long n)
{
	int		count;
	char	*hex_digits;

	hex_digits = "0123456789abcdef";
	count = 0;
	if (n >= 16)
		count += ft_puthex(n / 16);
	count += ft_putchar(hex_digits[n % 16]);
	return (count);
}
