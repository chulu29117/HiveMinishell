/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:59:10 by clu               #+#    #+#             */
/*   Updated: 2025/04/22 16:37:56 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Malloc with error handling.
*/
void	*x_malloc(t_ms *ms, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		ft_putstr_fd("malloc failed\n", STDERR_FILENO);
		bi_exit(ms, 1, 1);
	}
	return (ptr);
}

/*
** Strdup with error handling.
*/
char	*x_strdup(t_ms *ms, const char *s)
{
	char	*dup;

	dup = ft_strdup(s);
	if (!dup)
	{
		ft_putstr_fd("ft_strdup failed\n", STDERR_FILENO);
		bi_exit(ms, 1, 1);
	}
	return (dup);
}

/*
** Substr with error handling.
*/
char	*x_substr(t_ms *ms, const char *s, unsigned int start, size_t len)
{
	char	*sub;

	sub = ft_substr(s, start, len);
	if (!sub)
	{
		ft_putstr_fd("ft_substr failed", STDERR_FILENO);
		bi_exit(ms, 1, 1);
	}
	return (sub);
}

/*
** Strjoin and free with error handling.
*/
char	*x_strjoin_free(t_ms *ms, char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	if (!joined)
	{
		free(s1);
		free(s2);
		ft_putstr_fd("ft_strjoin failed", STDERR_FILENO);
		bi_exit(ms, 1, 1);
	}
	free(s1);
	free(s2);
	return (joined);
}
