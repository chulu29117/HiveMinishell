/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:15:11 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/09 17:36:51 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Frees strings here because norm
*/
void	shlvl_free(char *s1, char *s2)
{
	free(s1);
	free(s2);
}

/*
** Calls ft_strjoin with variable name and value
** Frees value
** Handles error
*/
char	*shlvl_join(t_ms *ms, char *s1, char *s2)
{
	char	*temp;

	temp = ft_strjoin(s1, s2);
	if (!temp)
	{
		free(s2);
		ms_error(ms, "ft_strjoin failure", 1, 0);
	}
	return (temp);
}

/*
** Searches for SHLVL in envp
** If unset then creates variable with value 1
** Else increments value by 1 and resets variable
** If value is > 1000 then value reset to 1
** If value is < 0 then value reset to 0
*/
void	shlvl(t_ms *ms)
{
	char	*lvlstr;
	int		lvlint;
	char	*temp;

	lvlstr = find_var(ms, ms->envp, "SHLVL");
	if (!lvlstr)
	{
		export_var_to_envp(ms, "SHLVL=1");
		return ;
	}
	lvlint = ft_atoi(lvlstr);
	lvlstr = NULL;
	if (++lvlint > 1000)
	{
		printf(SHLV, lvlint);
		lvlint = 1;
	}
	else if (lvlint < 0)
		lvlint = 0;
	lvlstr = ft_itoa(lvlint);
	if (!lvlstr)
		ms_error(ms, "ft_itoa failire", 1, 0);
	temp = shlvl_join(ms, "SHLVL=", lvlstr);
	export_var_to_envp(ms, temp);
	shlvl_free(temp, lvlstr);
}
