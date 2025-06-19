/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   only_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:19:21 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/22 14:58:55 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Resets stdin if fd_in is open
** Resets stout if fd_out is open
*/
void	reset_std(t_ms *ms)
{
	if (ms->reset[0] == 1)
	{
		if (dup2(ms->std_copy[0], STDIN_FILENO) < 0)
			ms_error(ms, "stdin reset failed", 1, 0);
		close(ms->std_copy[0]);
		ms->reset[0] = 0;
	}
	if (ms->reset[1] == 1)
	{
		if (dup2(ms->std_copy[1], STDOUT_FILENO) < 0)
			ms_error(ms, "stdout reset failed", 1, 0);
		close(ms->std_copy[1]);
		ms->reset[1] = 0;
	}
}

/*
** Child process to handle input that is only redirection
*/
void	run_redir(t_ms *ms, t_ast *ast)
{
	ms->pids[ms->pid_index++] = fork();
	if (ms->pids[ms->pid_index - 1] < 0)
		ms_error(ms, "Fork failure", 1, 0);
	else if (ms->pids[ms->pid_index - 1])
		return (hold_signals());
	reset_signals();
	if (handle_files(ms, ast->cmd))
		bi_exit(ms, ms->exit_status, 1);
	if (dup2(ms->ms_fd[1], 1) < 0)
		full_error_msg(ms, ast->cmd->full_cmd[0], strerror(errno));
	reset_std(ms);
	close_fd(ms);
	bi_exit(ms, 0, 1);
}

/*
** Parent process control for only redirection
*/
void	run_no_cmd(t_ms *ms, t_ast *ast)
{
	if (pipe(ms->ms_fd) < 0)
		ms_error(ms, "Pipe failure", 1, 0);
	run_redir(ms, ast);
	if (ms->cmd_no > 1)
		next_pipe(ms, ms->cmd_index + 1 >= ms->cmd_no);
	reset_std(ms);
}

void	wait_help(t_ms *ms)
{
	waitpid(ms->pids[ms->pid_index], &ms->exit_status, 0);
	if (WIFEXITED(ms->exit_status))
		ms->exit_status = WEXITSTATUS(ms->exit_status);
	else if (WIFSIGNALED(ms->exit_status))
	{
		ms->exit_status = 128 + WTERMSIG(ms->exit_status);
		if (ms->exit_status == 131 && ms->pid_index == ms->cmd_no - 1)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		else if (ms->exit_status != 131 && ms->pid_index == ms->cmd_no - 1)
			write(STDERR_FILENO, "\n", 1);
	}
}
