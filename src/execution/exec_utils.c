/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:21:16 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/22 15:05:10 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Joins both arguments to one string for child safe printing
** Calls ms_error()
*/
void	full_error_msg(t_ms *ms, char *s1, char *s2)
{
	char	*err;
	char	*temp;

	temp = ft_strjoin(": ", s2);
	if (!temp)
		ms_error(ms, "error message error", 1, 0);
	err = ft_strjoin(s1, temp);
	if (!err)
	{
		free(temp);
		ms_error(ms, "error message error", 1, 0);
	}
	free(temp);
	ms_error(ms, err, ms->exit_status, 1);
}

/*
** Finds executable by checking against PATH variable
** If match not found in PATH or PATH unset the variable is returned
*/
char	*get_path(t_ms *ms, t_cmd *cmd, char **envp, int i)
{
	char	*exe;
	char	**full_path;
	char	*part_path;
	char	*path;

	path = find_var(ms, envp, "PATH");
	if (!path)
		return (NULL);
	full_path = ft_split(path, ':');
	if (!full_path)
		ms_error(ms, "PATH split allocation failure", 1, 0);
	while (full_path[++i])
	{
		part_path = ft_strjoin(full_path[i], "/");
		exe = ft_strjoin(part_path, cmd->full_cmd[0]);
		free(part_path);
		if (!access(exe, F_OK))
		{
			ft_free_array(full_path);
			return (exe);
		}
		free(exe);
	}
	ft_free_array(full_path);
	return (NULL);
}

/*
** Checks that commands are not directories
** Checks if command is empty string
*/
void	validation(t_ms *ms, t_cmd *cmd)
{
	int	dir;

	if (!cmd->full_cmd || !cmd->full_cmd[0])
		return ;
	if (cmd->full_cmd[0][0] == 0)
	{
		if (cmd->from_expand)
			return ;
		ms->exit_status = 127;
		full_error_msg(ms, cmd->full_cmd[0], "command not found");
	}
	dir = open(cmd->full_cmd[0], __O_DIRECTORY);
	if (dir != -1)
	{
		close(dir);
		ms->exit_status = 126;
		full_error_msg(ms, cmd->full_cmd[0], "Is a directory");
	}
	if (ft_strchr(cmd->full_cmd[0], '/'))
		ms->cmd_path = cmd->full_cmd[0];
	else
		find_exe(ms, cmd);
}

/*
** Prints out error message
** Will free error message if allocated
** Exits with given exit code
*/
void	ms_error(t_ms *ms, char *msg, int ex_code, int free_msg)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	else
		ft_putstr_fd("Failed to get error message\n", 2);
	if (free_msg == 1)
		free(msg);
	bi_exit(ms, ex_code, 1);
}

/*
** Closes any open file desciptors
** Calls close_pipes()
*/
void	close_fd(t_ms *ms)
{
	if (ms->fd_in != -1)
	{
		close(ms->fd_in);
		ms->fd_in = -1;
	}
	if (ms->fd_out != -1)
	{
		close(ms->fd_out);
		ms->fd_out = -1;
	}
	close_pipes(ms);
}
