/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_master.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:34:17 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/22 14:53:29 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Handles any redirection if present
** Returns 0 on success
** Returns 1 on any redirection failure
*/
int	handle_files(t_ms *ms, t_cmd *cmd)
{
	if (cmd->infile && g_signal != SIGINT)
		if (handle_infiles(ms, cmd->infile) < 0)
			return (1);
	if (cmd->outfile && g_signal != SIGINT)
		if (handle_outfiles(ms, cmd->outfile, cmd->append) < 0)
			return (1);
	return (0);
}

/*
** Begins heredoc by creating temporary file
** Changes behaviour of SIGINT while heredoc is open
** Behaviour of SIGINT reset when heredoc closed
*/
int	start_heredoc(t_ms *ms, char *lim, t_infile *infile, int quo)
{
	ms->heredoc_name = heredoc_name(ms, ms->heredoc_no);
	signal(SIGINT, handle_signal);
	rl_event_hook = heredoc_sigint;
	if (handle_heredoc(ms, lim, ms->heredoc_name, quo) == -1)
	{
		unlink(ms->heredoc_name);
		free(ms->heredoc_name);
		ms->heredoc_name = NULL;
		ms->heredoc_no--;
		return (1);
	}
	rl_event_hook = NULL;
	if (infile->name)
	{
		free(infile->name);
		infile->name = NULL;
	}
	infile->name = ms->heredoc_name;
	ms->heredoc_name = NULL;
	return (0);
}

/*
** Closes any open pipes if open
** Resets any closed pipes to -1
*/
void	close_pipes(t_ms *ms)
{
	if (ms->ms_fd[0] != -1)
	{
		close(ms->ms_fd[0]);
		ms->ms_fd[0] = -1;
	}
	if (ms->ms_fd[1] != -1)
	{
		close(ms->ms_fd[1]);
		ms->ms_fd[1] = -1;
	}
	if (ms->prev_fd != -1)
	{
		close(ms->prev_fd);
		ms->prev_fd = -1;
	}
}

/*
** Initialises all variables used in executor
** Variables will be reset on each readline call
*/
void	init_executor(t_ms *ms)
{
	ms->fd_in = -1;
	ms->fd_out = -1;
	ms->pids = NULL;
	ms->pid_index = 0;
	ms->cmd_path = NULL;
	ms->std_copy[0] = -1;
	ms->std_copy[1] = -1;
	ms->ms_fd[0] = -1;
	ms->ms_fd[1] = -1;
	ms->prev_fd = -1;
	ms->cmd_index = 0;
	ms->child = true;
	ms->reset[0] = 0;
	ms->reset[1] = 0;
	ms->infile_index = 0;
}

/*
** Control structure for execution
** Allocates array of process IDs
** Sends ast to ast parser for execution
** Waits for any child processes to finish
** Frees heredoc memory and PID array
*/
void	run_executor(t_ms *ms, int i)
{
	init_executor(ms);
	ms->pids = malloc(ms->cmd_no * sizeof(pid_t));
	if (!ms->pids)
		ms_error(ms, "PID array malloc fail", 1, 0);
	while (++i < ms->cmd_no)
		ms->pids[i] = -1;
	parse_cmds(ms, ms->ast);
	close_fd(ms);
	ms->pid_index = -1;
	if (ms->child == true)
	{
		while (++ms->pid_index < ms->cmd_no)
			wait_help(ms);
		set_signals();
	}
	free_heredoc(ms);
	free(ms->pids);
	ms->pids = NULL;
	if (ms->cmd_no == 1)
		update_env(ms);
}
