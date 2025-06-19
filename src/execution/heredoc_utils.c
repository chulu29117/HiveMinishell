/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:21:33 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/20 14:12:01 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Creates unique heredoc name
** Starts at .heredoc0
*/
char	*heredoc_name(t_ms *ms, int i)
{
	char	*name;
	char	*no;

	no = ft_itoa(i);
	if (!no)
		ms_error(ms, "ft_itoa failure", 1, 0);
	name = ft_strjoin(".heredoc", no);
	free(no);
	if (!name)
		ms_error(ms, "heredoc file generation failure", 1, 0);
	return (name);
}

/*
** Part of redirect_out to comply with norm rules
*/
int	redirect_out_helper(t_ms *ms, char *file, int dir)
{
	close(dir);
	ms->exit_status = 1;
	error_join(ms, file, ": Is a directory");
	ms->fd_in = -1;
	return (-1);
}

/*
** Closes open heredoc file
*/
void	free_heredoc(t_ms *ms)
{
	if (ms->fd_in != -1)
	{
		close(ms->fd_in);
		ms->fd_in = -1;
	}
}

/*
** Helper because of norm
*/
void	heredoc_help(t_ms *ms, char *line, int fd, int quo)
{
	char	*expanded;
	bool	was_expanded;

	was_expanded = false;
	if (quo == UNQUOTED)
	{
		expanded = expand_input(ms, line, &was_expanded);
		free(line);
		line = expanded;
	}
	ft_putstr_fd(line, fd);
	ft_putstr_fd("\n", fd);
	free(line);
}
