/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 12:00:47 by clu               #+#    #+#             */
/*   Updated: 2025/01/15 16:22:19 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char		*ptr;
	size_t		len;
	size_t		i;

	if (s1 == NULL)
		return (NULL);
	len = ft_strlen(s1) + 1;
	ptr = (char *)malloc(len * sizeof(char));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < len - 1)
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
