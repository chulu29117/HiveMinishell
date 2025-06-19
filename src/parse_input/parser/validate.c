/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:59:16 by clu               #+#    #+#             */
/*   Updated: 2025/05/18 02:30:33 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	validate_basic_syntax(t_ms *ms, t_token *tokens, t_token *last);
static bool	validate_redirection_syntax(t_ms *ms, t_token *tokens);
static bool	syntax_error(t_ms *ms, char *msg);

/*
** Validate the input tokens for syntax errors.
** Check for basic syntax errors about pipes.
** Check for redirection syntax errors.
** Check for heredoc limit.
*/
bool	validate_input(t_ms *ms, t_token *tokens)
{
	t_token	*current;
	t_token	*last;

	last = NULL;
	if (ms->syntax_error)
	{
		clean_all_heredocs(ms);
		return (false);
	}
	current = tokens;
	while (current)
	{
		last = current;
		current = current->next;
	}
	if (!validate_basic_syntax(ms, tokens, last))
		return (false);
	if (!validate_redirection_syntax(ms, tokens))
		return (false);
	if (!check_heredoc_limit(ms))
	{
		clean_all_heredocs(ms);
		return (false);
	}
	return (true);
}

/*
** Validate basic syntax rules like pipes at the beginning/end.
** Check for consecutive pipes and redirection tokens.
*/
static bool	validate_basic_syntax(t_ms *ms, t_token *tokens, t_token *last)
{
	t_token	*current;

	current = tokens;
	while (current && current->next)
	{
		if (current->type == TOKEN_PIPE && current->next->type == TOKEN_PIPE)
			return (syntax_error(ms, "`||'\n"));
		current = current->next;
	}
	current = tokens;
	if (tokens && tokens->type == TOKEN_PIPE)
		return (syntax_error(ms, "`|'\n"));
	if (last && last->type == TOKEN_PIPE)
		return (syntax_error(ms, "`|'\n"));
	if (last && (last->type == TOKEN_REDIR_IN || last->type == TOKEN_REDIR_OUT
			|| last->type == TOKEN_APPEND || last->type == TOKEN_HEREDOC))
		return (syntax_error(ms, "`newline'\n"));
	return (true);
}

/*
** Check if redirection tokens are followed by a word token.
*/
static bool	validate_redirection_syntax(t_ms *ms, t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->next)
	{
		if ((current->type == TOKEN_REDIR_IN
				|| current->type == TOKEN_REDIR_OUT
				|| current->type == TOKEN_APPEND
				|| current->type == TOKEN_HEREDOC)
			&& (current->next->type != TOKEN_WORD))
		{
			if (current->next->type == TOKEN_PIPE)
				return (syntax_error(ms, "`|'\n"));
			else
				return (syntax_error(ms, "`newline'\n"));
		}
		current = current->next;
	}
	return (true);
}

/*
** Handling of the syntax error and heredoc cleanup.
*/
static bool	syntax_error(t_ms *ms, char *msg)
{
	ft_putstr_fd(SYNTAX, STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	clean_all_heredocs(ms);
	ms->exit_status = 2;
	return (false);
}

/*
** Clean up all heredoc files created during the session.
*/
void	clean_all_heredocs(t_ms *ms)
{
	int		i;
	char	*filename;
	char	*temp;

	if (!ms)
		return ;
	i = 0;
	while (i <= ms->heredoc_no)
	{
		temp = ft_itoa(i);
		if (!temp)
			return ;
		filename = ft_strjoin(".heredoc", temp);
		free(temp);
		if (filename)
		{
			unlink(filename);
			free(filename);
		}
		i++;
	}
	ms->heredoc_no = 0;
}
