/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:03:02 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/15 14:47:21 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Prints arguments with no new line
** Resets exit code to 0
*/
int	echo_nprinting(t_ms *ms, char **lines)
{
	int	i;

	i = 0;
	while (lines[++i])
	{
		write(1, lines[i], ft_strlen(lines[i]));
		if (lines[i + 1])
			write(1, " ", 1);
	}
	ms->exit_status = 0;
	return (1);
}

/*
** Prints arguments with new line
** Resets exit code to 0
*/
int	echo_printing(t_ms *ms, char **lines)
{
	int	i;

	i = 0;
	while (lines[++i])
	{
		write(1, lines[i], ft_strlen(lines[i]));
		if (lines[i + 1])
			write(1, " ", 1);
	}
	write(1, "\n", 1);
	ms->exit_status = 0;
	return (1);
}

/*
** Checks if -n flag has been given
** Checks the flag only contains n
** Calls appropriate printing function based on flag
*/
int	echo(t_ms *ms, char **lines, bool nl, int flags)
{
	int		i;
	int		j;

	i = 0;
	if (!lines || !lines[1])
		return (write(1, "\n", 1));
	while (lines[++i] && lines[i][0] == '-' && lines[i][1] == 'n')
	{
		nl = false;
		j = 0;
		while (lines[i][++j])
		{
			if (lines[i][j] != 'n')
			{
				if (flags == 0)
					nl = true;
				else
					flags--;
				break ;
			}
		}
		flags++;
	}
	return ((nl && echo_printing(ms, lines))
		|| echo_nprinting(ms, lines + flags));
}
