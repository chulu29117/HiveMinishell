/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 16:40:42 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/22 13:56:01 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Prepares pipe for next child process
** Creates new pipe if more than one process is left
*/
void	next_pipe(t_ms *ms, int last)
{
	if (ms->prev_fd != -1)
		close(ms->prev_fd);
	close(ms->ms_fd[1]);
	ms->ms_fd[1] = -1;
	ms->prev_fd = ms->ms_fd[0];
	ms->ms_fd[0] = -1;
	if (last)
		return ;
	if (pipe(ms->ms_fd) < 0)
		ms_error(ms, "Pipe failiure", 1, 0);
}

/*
** Function to run first command in pipeline
*/
void	exec_first_pipe(t_ms *ms, t_ast *ast)
{
	ms->pids[ms->pid_index++] = fork();
	if (ms->pids[ms->pid_index - 1] < 0)
		ms_error(ms, "Fork failure", 1, 0);
	else if (ms->pids[ms->pid_index - 1])
		return (hold_signals());
	reset_signals();
	if (ast->cmd->infile)
	{
		if (handle_infiles(ms, ast->cmd->infile) < 0)
			bi_exit(ms, 130, 1);
	}
	if (ast->cmd->outfile)
	{
		if (handle_outfiles(ms, ast->cmd->outfile, ast->cmd->append) < 0)
			bi_exit(ms, 130, 1);
	}
	else
	{
		if (dup2(ms->ms_fd[1], 1) < 0)
			full_error_msg(ms, ast->cmd->full_cmd[0], strerror(errno));
	}
	run_cmd(ms, ast->cmd);
	bi_exit(ms, 0, 1);
}

/* 
** Function to run commands mid pipeline (not first or last)
*/
void	exec_mid_pipe(t_ms *ms, t_ast *ast)
{
	ms->pids[ms->pid_index++] = fork();
	if (ms->pids[ms->pid_index - 1] < 0)
		ms_error(ms, "Fork failure", 1, 0);
	else if (ms->pids[ms->pid_index - 1])
		return (hold_signals());
	reset_signals();
	if (ast->cmd->infile)
	{
		if (handle_infiles(ms, ast->cmd->infile) < 0)
			bi_exit(ms, 130, 1);
	}
	else
		if (dup2(ms->prev_fd, 0) < 0)
			full_error_msg(ms, ast->cmd->full_cmd[0], strerror(errno));
	if (ast->cmd->outfile)
	{
		if (handle_outfiles(ms, ast->cmd->outfile, ast->cmd->append) < 0)
			bi_exit(ms, 130, 1);
	}
	else
		if (dup2(ms->ms_fd[1], 1) < 0)
			full_error_msg(ms, ast->cmd->full_cmd[0], strerror(errno));
	run_cmd(ms, ast->cmd);
	bi_exit(ms, 0, 1);
}

/*
** Function to run final command in pipeline
*/
void	exec_last_pipe(t_ms *ms, t_ast *ast)
{
	ms->pids[ms->pid_index++] = fork();
	if (ms->pids[ms->pid_index - 1] < 0)
		ms_error(ms, "Fork failure", 1, 0);
	else if (ms->pids[ms->pid_index - 1])
		return (hold_signals());
	reset_signals();
	if (ast->cmd->infile)
	{
		close(ms->prev_fd);
		ms->prev_fd = -1;
		if (handle_infiles(ms, ast->cmd->infile) < 0)
			bi_exit(ms, 130, 1);
	}
	else
		if (dup2(ms->prev_fd, 0) < 0)
			full_error_msg(ms, ast->cmd->full_cmd[0], strerror(errno));
	if (ast->cmd->outfile)
	{
		if (handle_outfiles(ms, ast->cmd->outfile, ast->cmd->append) < 0)
			bi_exit(ms, 130, 1);
	}
	run_cmd(ms, ast->cmd);
	bi_exit(ms, 0, 1);
}

/*
** Pipeline control structure
** Prepares pipes for next process
*/
void	run_pipes(t_ms *ms, t_ast *ast)
{
	if (pipe(ms->ms_fd) < 0)
		ms_error(ms, "Pipe failure", 1, 0);
	if (ms->cmd_index == 1)
	{
		exec_first_pipe(ms, ast);
		next_pipe(ms, ms->cmd_index + 1 >= ms->cmd_no);
	}
	else if (ms->cmd_index == ms->cmd_no)
		exec_last_pipe(ms, ast);
	else
	{
		exec_mid_pipe(ms, ast);
		next_pipe(ms, ms->cmd_index + 1 >= ms->cmd_no);
	}
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
	free_heredoc(ms);
}
