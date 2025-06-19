/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:23:14 by mpierce           #+#    #+#             */
/*   Updated: 2025/04/24 17:42:02 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_heredoc_sigint(t_ms *ms, char *name)
{
	(void)ms;
	g_signal = 0;
	unlink(name);
	free(name);
}

void	update_env(t_ms *ms)
{
	int		i;
	char	*temp;
	char	*temp2;
	char	*temp3;

	i = -1;
	if (ms->ast->cmd->full_cmd)
	{
		while (ms->ast->cmd->full_cmd[++i])
			;
		temp = ms->ast->cmd->full_cmd[i - 1];
	}
	else
		temp = "";
	temp2 = ft_strjoin("_=", temp);
	if (!temp2)
		ms_error(ms, "ft_strjoin failure", 1, 0);
	temp3 = x_strdup(ms, "_");
	run_export(ms, temp2, temp3, false);
	free(temp2);
}
