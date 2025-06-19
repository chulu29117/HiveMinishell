/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 16:48:47 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/16 13:37:40 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Checks if given infile name is empty string
** Checks if given infile is a directory
** Checks if given infile can be opened
** Returns -1 on error and 0 on success
*/
int	redirect_in(t_ms *ms, char *file)
{
	int	dir;

	if (*file == 0)
	{
		error_join(ms, file, ": No such file or directory");
		return (-1);
	}
	dir = open(file, __O_DIRECTORY);
	if (dir != -1)
	{
		close(dir);
		ms->exit_status = 2;
		error_join(ms, file, ": Is a directory");
		ms->fd_in = -1;
		return (-1);
	}
	ms->fd_in = open(file, O_RDWR, 0644);
	if (ms->fd_in == -1)
	{
		close_fd(ms);
		ms->exit_status = 1;
		error_join(ms, file, strerror(errno));
		return (-1);
	}
	return (0);
}

/*
** Checks if given outfile name is empty string
** Checks if given outfile is a directory
** Checks if given outfile can be opened
** Will open with either truncate or append flag
** Returns -1 on error and 0 on success
*/
static int	redirect_out(t_ms *ms, char *file, int *append, int *i)
{
	int	dir;

	if (*file == 0)
	{
		error_join(ms, file, ": No such file or directory");
		return (-1);
	}
	dir = open(file, __O_DIRECTORY);
	if (dir != -1)
		return (redirect_out_helper(ms, file, dir));
	if (append[*i] == 0)
		ms->fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		ms->fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (ms->fd_out == -1)
	{
		close_fd(ms);
		ms->exit_status = 1;
		error_join(ms, file, strerror(errno));
		return (-1);
	}
	return (0);
}

/*
** Iterates through infile array, opening each file
** Uses dup2() to replace stdin
** Checks if redirect is file or heredoc
** Final file's fd will be stdin
** Returns -1 on error and 0 on success
*/
int	handle_infiles(t_ms *ms, t_infile **infile)
{
	int	i;

	i = -1;
	ms->std_copy[0] = dup(STDIN_FILENO);
	if (ms->std_copy[0] < 0)
		ms_error(ms, "stdin duplication failed", 1, 0);
	ms->reset[0] = 1;
	while (infile[++i])
	{
		if (redirect_in(ms, infile[i]->name) < 0)
			return (-1);
		if (infile[i]->heredoc == 1)
			unlink(infile[i]->name);
		if (dup2(ms->fd_in, STDIN_FILENO) < 0)
			ms_error(ms, "stdin redirection failed", 1, 0);
		ms->reset[0] = 1;
		close(ms->fd_in);
		ms->fd_in = -1;
	}
	return (0);
}

/*
** Iterates through outfile array, opening each file
** Uses dup2() to replace stdout
** Final file's fd will be stdout
** Returns -1 on error and 0 on success
*/
int	handle_outfiles(t_ms *ms, char **outfile, int *append)
{
	int	i;

	i = -1;
	ms->std_copy[1] = dup(STDOUT_FILENO);
	if (ms->std_copy[1] < 0)
		ms_error(ms, "stdout duplication failed", 1, 0);
	while (outfile[++i])
	{
		if (redirect_out(ms, outfile[i], append, &i) < 0)
			return (-1);
		if (dup2(ms->fd_out, STDOUT_FILENO) < 0)
			ms_error(ms, "stdout redirection failed", 1, 0);
		ms->reset[1] = 1;
	}
	return (0);
}

/*
** Creates temporary file for heredoc
** Each heredoc will have unique name
** Handles SIGINT for open heredoc
** Returns file descriptor on success and -1 on SIGINT
*/
int	handle_heredoc(t_ms *ms, const char *heredoc, char *name, int quo)
{
	char	*line;
	int		fd;
	char	*temp;

	temp = NULL;
	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		ms_error(ms, "Failed to create temporary file for HEREDOC", 1, 0);
	while (1)
	{
		if (isatty(fileno(stdin)))
			line = readline("> ");
		else
			line = get_line(temp, line);
		if (!line || ft_strcmp(line, heredoc) == 0 || g_signal == SIGINT)
		{
			if (readline_break(line, fd) < 0)
				return (-1);
			break ;
		}
		heredoc_help(ms, line, fd, quo);
	}
	close(fd);
	return (0);
}
