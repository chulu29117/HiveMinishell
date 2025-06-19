/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:58:46 by clu               #+#    #+#             */
/*   Updated: 2025/05/22 14:20:46 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Detects if cmd is a builtin and runs internally
** If not a builtin it will be passed to execve
*/
void	run_cmd(t_ms *ms, t_cmd *cmd)
{
	if (!ft_strcmp(cmd->full_cmd[0], "cd"))
		parse_cd(ms, cmd->full_cmd);
	else if (!ft_strcmp(cmd->full_cmd[0], "env"))
		write_env(ms);
	else if (!ft_strcmp(cmd->full_cmd[0], "export"))
		run_export_args(ms, cmd->full_cmd);
	else if (!ft_strcmp(cmd->full_cmd[0], "unset"))
		run_unset(ms, cmd);
	else if (!ft_strcmp(cmd->full_cmd[0], "echo"))
		echo(ms, cmd->full_cmd, true, 0);
	else if (!ft_strcmp(cmd->full_cmd[0], "pwd"))
		write_pwd(ms);
	else if (!ft_strcmp(cmd->full_cmd[0], "exit"))
		exit_args(ms, cmd->full_cmd, -1);
	else
		run_execve(ms, cmd);
	if (ms->child == true)
	{
		close_pipes(ms);
		if (ms->reset[0] == 1)
			close(ms->std_copy[0]);
		if (ms->reset[1] == 1)
			close(ms->std_copy[1]);
	}
}

/*
** Checks if cmd is a builtin
** Returns 1 if yes and 0 if not 
*/
int	check_for_bi(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->full_cmd[0], "cd"))
		return (1);
	else if (!ft_strcmp(cmd->full_cmd[0], "env"))
		return (1);
	else if (!ft_strcmp(cmd->full_cmd[0], "export"))
		return (1);
	else if (!ft_strcmp(cmd->full_cmd[0], "unset"))
		return (1);
	else if (!ft_strcmp(cmd->full_cmd[0], "echo"))
		return (1);
	else if (!ft_strcmp(cmd->full_cmd[0], "pwd"))
		return (1);
	else if (!ft_strcmp(cmd->full_cmd[0], "exit"))
		return (1);
	else
		return (0);
}

/*
** Control structure for a single cmd
** Runs in parent if a builtin
** Otherwise runs process in child and sends to execve
*/
void	run_one(t_ms *ms, t_cmd *cmd)
{
	if (check_for_bi(cmd) == 1)
	{
		ms->child = false;
		if (handle_files(ms, cmd))
			return ;
		if (g_signal == SIGINT)
			return ;
		run_cmd(ms, cmd);
		return ;
	}
	ms->pids[ms->pid_index++] = fork();
	if (ms->pids[ms->pid_index - 1] < 0)
		ms_error(ms, "Fork failure", 1, 0);
	else if (ms->pids[ms->pid_index - 1])
		return (hold_signals());
	reset_signals();
	if (handle_files(ms, cmd) || g_signal)
		bi_exit(ms, 1, 1);
	if (g_signal)
		bi_exit(ms, 128 + g_signal, 1);
	run_cmd(ms, cmd);
}

/*
** Checks whether operation is part of a pipeline
** If pipeline exists will run all in child process
** Resets stdin and stdout if modified 
*/
void	start_cmds(t_ms *ms, t_ast *ast)
{
	if (ms->cmd_no == 1)
		run_one(ms, ast->cmd);
	else
		run_pipes(ms, ast);
	if (g_signal != SIGINT)
		reset_std(ms);
}

/*
** Recursive function to parse ast
** Will process all on the left side first then right side
*/
void	parse_cmds(t_ms *ms, t_ast *ast)
{
	if (!ast)
		return ;
	while (ast && g_signal != SIGINT)
	{
		if (ast->type == NODE_PIPE)
		{
			parse_cmds(ms, ast->left);
			ast = ast->right;
		}
		else if (ast->type == NODE_COMMAND)
		{
			ms->cmd_index++;
			start_cmds(ms, ast);
			return ;
		}
		else if (ast->type == NODE_MISSCMD)
		{
			ms->cmd_index++;
			run_no_cmd(ms, ast);
			return ;
		}
	}
}
