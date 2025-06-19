/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:32:52 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/20 12:53:28 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	error_join(t_ms *ms, char *name, char *error)
{
	char	*temp;
	char	*temp2;

	temp = ft_strjoin(name, ": ");
	if (!temp)
		ms_error(ms, "ft_strjoin failure", 1, 0);
	temp2 = ft_strjoin(temp, error);
	if (!temp2)
	{
		free(temp);
		ms_error(ms, "ft_strjoin failure", 1, 0);
	}
	ft_putendl_fd(temp2, 2);
	free(temp);
	free(temp2);
}

char	*exit_join(t_ms *ms, char *name)
{
	char	*temp;
	char	*temp2;

	temp = ft_strjoin("minishell: exit: ", name);
	if (!temp)
		ms_error(ms, "ft_strjoin failure", 1, 0);
	temp2 = ft_strjoin(temp, ": numeric argument required\n");
	if (!temp2)
	{
		free(temp);
		ms_error(ms, "ft_strjoin failure", 1, 0);
	}
	free(temp);
	return (temp2);
}

void	exit_args(t_ms *ms, char **full_cmd, int i)
{
	char	*temp;

	while (full_cmd[++i])
		;
	if (i > 2)
	{
		i = -1;
		while (full_cmd[1][++i])
		{
			if (!ft_isdigit(full_cmd[1][i]))
			{
				temp = exit_join(ms, full_cmd[1]);
				ft_putstr_fd(temp, 2);
				free(temp);
				bi_exit(ms, 2, 0);
			}
		}
		ms->exit_status = 1;
		return (ft_putstr_fd("minishell: exit: too many arguments\n", 2));
	}
	if (full_cmd[1])
		check_valid_arg_exit(ms, full_cmd[1]);
	else
		bi_exit(ms, ms->exit_status, 0);
}

/*
** Validates commands and passes to execve
** Exits in event execve fails
** Sets exit status to 126 if permission error
** Sets exit status to 127 in all other erros
*/
void	run_execve(t_ms *ms, t_cmd *cmd)
{
	validation(ms, cmd);
	if (cmd->from_expand && (!cmd->full_cmd || !cmd->full_cmd[0]
			|| cmd->full_cmd[0][0] == '\0'))
		bi_exit(ms, 0, 1);
	close_fd(ms);
	if (ms->reset[0] == 1)
		close(ms->std_copy[0]);
	if (ms->reset[1] == 1)
		close(ms->std_copy[1]);
	execve(ms->cmd_path, cmd->full_cmd, ms->envp);
	if (errno == EACCES)
		ms->exit_status = 126;
	else
		ms->exit_status = 127;
	full_error_msg(ms, cmd->full_cmd[0], strerror(errno));
}
// ps -eAo state,pid,comm | grep -w Z *finds zombies*
