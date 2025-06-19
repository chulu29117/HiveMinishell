/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:50:02 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/22 14:19:35 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_signals(void)
{
	struct sigaction	act;
	struct sigaction	sigint;

	ft_memset(&act, 0, sizeof(act));
	ft_memset(&sigint, 0, sizeof(sigint));
	act.sa_handler = SIG_IGN;
	sigint.sa_sigaction = do_sigint;
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGINT, &sigint, NULL);
}

/*
** Assigns the static pointer minishell to the regular struct
*/
t_ms	*get_minishell(t_ms *ms)
{
	static t_ms	*minishell;

	if (ms)
	{
		minishell = ms;
		return (NULL);
	}
	return (minishell);
}

/*
** Assigned received signal value to global variable
*/
void	handle_signal(int sig)
{
	g_signal = sig;
}

/*
** Sets variable inside readline to end heredoc on SIGINT
*/
int	heredoc_sigint(void)
{
	if (g_signal == SIGINT)
	{
		rl_done = 1;
	}
	return (0);
}

/*
** Writes newline and sets readline to make new prompt on SIGINT
** Resets global variable to 0
*/
void	do_sigint(int a, siginfo_t *b, void *c)
{
	(void)a;
	(void)b;
	(void)c;
	get_minishell(NULL)->exit_status = 128 + SIGINT;
	g_signal = 0;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
