/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_format_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:03:14 by clu               #+#    #+#             */
/*   Updated: 2025/01/27 21:30:04 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_format_check(char specifier, va_list args)
{
	if (specifier == 'c')
		return (ft_putchar(va_arg(args, int)));
	else if (specifier == 's')
		return (ft_putstr(va_arg(args, char *)));
	else if (specifier == 'p')
		return (ft_putptr(va_arg(args, void *)));
	else if (specifier == 'd' || specifier == 'i')
		return (ft_putnbr(va_arg(args, int)));
	else if (specifier == 'i')
		return (ft_putnbr(va_arg(args, int)));
	else if (specifier == 'x')
		return (ft_puthex_low(va_arg(args, unsigned int)));
	else if (specifier == 'X')
		return (ft_puthex_up(va_arg(args, unsigned int)));
	else if (specifier == 'u')
		return (ft_putnbr_i(va_arg(args, unsigned int)));
	else if (specifier == '%')
		return (ft_putchar('%'));
	return (0);
}
