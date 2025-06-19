/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_to_array.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:38:25 by clu               #+#    #+#             */
/*   Updated: 2025/04/22 15:18:00 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Append to array of strings.
** Allocates new memory for the new string and appends it to the array.
*/
int	ft_add_to_array(char ***array, const char *str)
{
	int		count;
	char	**new_array;

	count = ft_array_length(*array);
	new_array = ft_realloc(*array, sizeof(char *) * (count + 1),
			sizeof(char *) * (count + 2));
	if (!new_array)
		return (-1);
	new_array[count] = ft_strdup(str);
	if (!new_array[count])
	{
		free(new_array);
		return (-1);
	}
	new_array[count + 1] = NULL;
	*array = new_array;
	return (0);
}

int	ft_array_length(char **array)
{
	int	len;

	len = 0;
	while (array && array[len])
		len++;
	return (len);
}
