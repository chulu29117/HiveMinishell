/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:23:57 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/22 15:08:10 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	find_exe(t_ms *ms, t_cmd *cmd)
{
	if (!find_var(ms, ms->envp, "PATH"))
	{
		if (access(cmd->full_cmd[0], F_OK) == 0)
			ms->cmd_path = cmd->full_cmd[0];
		else
		{
			error_join(ms, cmd->full_cmd[0], "command not found");
			bi_exit(ms, 127, 1);
		}
	}
	else
	{
		ms->cmd_path = get_path(ms, cmd, ms->envp, -1);
		if (ms->cmd_path == NULL)
		{
			error_join(ms, cmd->full_cmd[0], "command not found");
			bi_exit(ms, 127, 1);
		}
	}
}

int	readline_break(char *line, int fd)
{
	if (line)
		free(line);
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		close(fd);
		return (-1);
	}
	return (0);
}

char	*get_line(char *temp, char *line)
{
	temp = get_next_line(fileno(stdin));
	line = ft_strtrim(temp, "\n");
	free(temp);
	return (line);
}

void	reset_signals(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_DFL;
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGINT, &act, NULL);
}

void	hold_signals(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGINT, &act, NULL);
}
