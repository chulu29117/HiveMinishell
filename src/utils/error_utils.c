/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:31:55 by clu               #+#    #+#             */
/*   Updated: 2025/04/16 17:29:13 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Sets the shell's exit status to 2, then prints a syntax error message.
*/
void	sys_error(t_ms *ms, char *error_msg)
{
	ms->exit_status = 2;
	ft_putstr_fd("Syntax error: ", STDERR_FILENO);
	ft_putstr_fd(error_msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

/*
** Prints an error message related to a command.
** It prints the command's name, then the error message, and if errno is set,
** calls perror() to show the system error.
*/
void	cmd_error(t_cmd *cmd, char *error_msg)
{
	ft_putstr_fd("Error: ", STDERR_FILENO);
	if (cmd && cmd->full_cmd && cmd->full_cmd[0])
	{
		ft_putstr_fd(cmd->full_cmd[0], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (error_msg)
	{
		ft_putstr_fd(error_msg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (errno)
		perror("");
}

/*
** A fatal error handler.
** Prints an error message (including command name if applicable),
** prints any system error (if errno is set), frees all allocated resources,
** clears the history, and exits the program.
*/
void	error(t_ms *ms, char *error_msg)
{
	ft_putstr_fd("Error: ", STDERR_FILENO);
	if (ms && ms->cmd && ms->cmd->full_cmd
		&& ms->cmd->full_cmd[0])
	{
		ft_putstr_fd(ms->cmd->full_cmd[0], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(error_msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (errno)
		perror("");
	if (ms)
		free_ms(ms);
	rl_clear_history();
	bi_exit(ms, 1, 1);
}

/*
** Prints an error message for a builtin command.
*/
void	builtin_err(t_ms *ms, char *error_msg)
{
	ft_putstr_fd("Error: ", STDERR_FILENO);
	if (ms && ms->cmd && ms->cmd->full_cmd && ms->cmd->full_cmd[0])
	{
		ft_putstr_fd(ms->cmd->full_cmd[0], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(error_msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}
