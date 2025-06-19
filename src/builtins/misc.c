/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:21:29 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/09 17:28:53 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Searches string for given variable
** If match is found a new array is created 
** Variable is removed from new array
** Old envp is freed 
*/
void	unset_envp(t_ms *ms, char *name)
{
	char	**temp;
	int		i;
	int		j;
	int		rm;

	i = 0;
	j = 0;
	if (!name || name[0] == 0 || (name[0] == '_' && name[1] == 0))
		return ;
	temp = malloc(ms->elements * sizeof (char *));
	if (!temp)
		ms_error(ms, "envp allocation failure", 1, 0);
	rm = find_in_envp(ms, ms->envp, name);
	if (rm < 0)
		return ;
	while (i < ms->elements)
	{
		if (i != rm)
			dup_envp(ms, temp, i, j++);
		i++;
	}
	ms->elements--;
	temp[j] = NULL;
	free_envp(ms);
	ms->envp = temp;
}

/*
** Passes each line of the envp to the main function
** Sets exit code to 0
*/
void	run_unset(t_ms *ms, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->full_cmd[++i])
		unset_envp(ms, cmd->full_cmd[i]);
	ms->exit_status = 0;
}

/* 
**If getcwd() fails, writes appropriate error 
*/
void	getcwd_error(t_ms *ms, char *s1, char *s2)
{
	char	*temp;

	temp = ft_strjoin(s1, s2);
	if (!temp)
		ms_error(ms, "error message error", 1, 0);
	full_error_msg(ms, temp, strerror(errno));
}

/*
** Writes the cwd 
** Sets exit status to 0
*/
int	write_pwd(t_ms *ms)
{
	char	*temp;

	(void)ms;
	temp = getcwd(NULL, 0);
	if (!temp)
	{
		temp = ft_strjoin("pwd: getcwd: ", strerror(errno));
		if (!temp)
			ms_error(ms, "ft_strjoin failure", 1, 0);
		ft_putendl_fd(temp, 2);
		free(temp);
		return (1);
	}
	printf("%s\n", temp);
	free(temp);
	ms->exit_status = 0;
	return (0);
}

/*
** Writes out each member of the envp
** If member has no value it is skipped
*/
void	write_env(t_ms *ms)
{
	int		i;
	int		j;
	bool	value;

	i = -1;
	run_export(ms, "_=/usr/bin/env", x_strdup(ms, "_"), false);
	while (ms->envp[++i])
	{
		j = -1;
		value = false;
		while (ms->envp[i][++j] && value == false)
			if (ms->envp[i][j] == '=')
				value = true;
		if (value == true)
			printf("%s\n", ms->envp[i]);
	}
	ms->exit_status = 0;
}
