/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:54:23 by clu               #+#    #+#             */
/*   Updated: 2025/05/19 00:41:29 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_complete_input(t_ms *ms);
static void	parse_and_execute(t_ms *ms);
static bool	quotes_balanced(const char *input);

/*
** Processes user input by reading and tokenizing it.
** Updates the token list and parses the input into an AST.
*/
void	process_input(t_ms *ms)
{
	char	*input;
	int		i;

	ms->syntax_error = false;
	input = get_complete_input(ms);
	if (!input)
		return ;
	i = 0;
	while (input[i] && ft_is_whitespace(input[i]))
		i++;
	if (!input[i])
	{
		free(input);
		return ;
	}
	if (!process_tokens(ms, input))
	{
		free_structs(ms);
		return ;
	}
	update_token(ms);
	parse_and_execute(ms);
}

/*
** Parse the input into an AST and execute the commands.
** Check for heredoc limits and handle errors.
** Execute the commands.
*/
static void	parse_and_execute(t_ms *ms)
{
	ms->ast = parse_input(&ms->token, ms);
	if (!ms->ast)
	{
		ms->exit_status = 2;
		return ;
	}
	if (ms->heredoc_no > 16)
	{
		ft_putstr_fd(HEREDOC_LIM, STDERR_FILENO);
		return ;
	}
	run_executor(ms, -1);
}

/*
** Read input from readline or stdin.
** Check for unclosed quotes and handle errors.
*/
static char	*get_complete_input(t_ms *ms)
{
	char	*input;
	char	*line;

	ms->cmd_no = 0;
	if (isatty(fileno(stdin)))
		input = readline(ms->prompt);
	else
	{
		line = get_next_line(fileno(stdin));
		input = ft_strtrim(line, "\n");
		free(line);
	}
	if (!input)
		bi_exit(ms, 0, 0);
	while (!quotes_balanced(input))
	{
		ms->syntax_error = true;
		ms->exit_status = 2;
		ft_putendl_fd("syntax error: unclosed quote", STDERR_FILENO);
		free(input);
		return (NULL);
	}
	ms->heredoc_no = 0;
	return (input);
}

/*
** Check if quotes are balanced in the input string.
*/
static bool	quotes_balanced(const char *input)
{
	int	i;
	int	single;
	int	dbl;

	i = 0;
	single = 0;
	dbl = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !dbl)
			single = !single;
		else if (input[i] == '"' && !single)
			dbl = !dbl;
		i++;
	}
	return (single == 0 && dbl == 0);
}
