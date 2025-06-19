/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:44:58 by clu               #+#    #+#             */
/*   Updated: 2025/05/20 13:38:15 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Update the token list by expanding the values of each token.
** If the token is a heredoc, it is flagged to not expand.
** If the token is a word, it is expanded.
** If the token is a redirection, it is checked for ambiguity.
** Retokenize the token list after expansion handling.
*/
void	update_token(t_ms *ms)
{
	t_token	*old_token;
	bool	check_heredoc;

	old_token = ms->token;
	while (old_token)
	{
		if (old_token->prev && old_token->prev->type == TOKEN_HEREDOC)
			check_heredoc = true;
		else
			check_heredoc = false;
		if (old_token->type == TOKEN_WORD)
			expand_token_value(ms, old_token, check_heredoc);
		old_token = old_token->next;
	}
	if (!check_ambiguous_redirect(ms))
	{
		ms->syntax_error = true;
		return ;
	}
	retokenize(ms);
}

/*
** Handles the expansion of a token based segments, heredoc, and quote state.
** If the token has segments, it is assembled into a single string.
** If the token is not a heredoc and is not quoted, it is expanded.
*/
void	expand_token_value(t_ms *ms, t_token *token, bool check_heredoc)
{
	char	*expanded;
	bool	was_expanded;

	was_expanded = false;
	if (token->segments)
	{
		expanded = assemble_token(ms, token, check_heredoc, &was_expanded);
		if (token->value)
			free(token->value);
		token->value = expanded;
		token->expanded = was_expanded;
	}
	else if (!check_heredoc && token->quoted != SINGLE_QUOTED && token->value)
	{
		expanded = expand_input(ms, token->value, &was_expanded);
		free(token->value);
		token->value = expanded;
		token->expanded = was_expanded;
	}
}

/*
** Assemble the token from its segments.
** If token is single quoted or heredoc, it is treated as a literal.
** If token is not quoted, it is expanded and expanded flag is set.
*/
char	*assemble_token(t_ms *ms, t_token *token, bool check_heredoc, bool *exp)
{
	char		*result;
	char		*part;	
	char		*temp;
	t_segment	*segments;

	result = x_strdup(ms, "");
	*exp = false;
	segments = token->segments;
	while (segments)
	{
		if (segments->quoted == SINGLE_QUOTED || check_heredoc)
			part = x_strdup(ms, segments->seg);
		else
		{
			part = expand_input(ms, segments->seg, exp);
			if (ft_strchr(part, '$'))
				*exp = true;
		}
		temp = x_strjoin_free(ms, result, part);
		result = temp;
		segments = segments->next;
	}
	return (result);
}

/*
** Processes the input string and expands variables.
** If $ is encountered, it expands the variable.
** Else, it processes the literal token.
*/
char	*expand_input(t_ms *ms, char *content, bool *was_expanded)
{
	int		pos;
	char	*result;
	char	*expanded;
	char	*temp;

	pos = 0;
	result = x_strdup(ms, "");
	if (was_expanded)
		*was_expanded = false;
	while (content[pos])
	{
		if (content[pos] == '$')
		{
			expanded = expand_var_token(ms, content, &pos);
			*was_expanded = true;
		}
		else
			expanded = expand_literal_token(ms, content, &pos);
		temp = x_strjoin_free(ms, result, expanded);
		result = temp;
	}
	return (result);
}

/*
** Check for ambiguous redirects.
** If a word token is preceded by a redirection token,
** and if word is unquoted and contains a space or is empty,
*/
bool	check_ambiguous_redirect(t_ms *ms)
{
	t_token	*current;

	current = ms->token;
	while (current)
	{
		if (current->type == TOKEN_WORD && current->prev
			&& (current->prev->type == TOKEN_REDIR_IN
				||current->prev->type == TOKEN_REDIR_OUT
				||current->prev->type == TOKEN_APPEND
				||current->prev->type == TOKEN_HEREDOC))
		{
			if ((current->quoted == UNQUOTED)
				&& (ft_strchr(current->value, ' ')
					|| current->value[0] == '\0'))
			{
				ft_putstr_fd("Error: ", STDERR_FILENO);
				ft_putstr_fd("ambiguous redirect\n", STDERR_FILENO);
				ms->exit_status = 1;
				return (false);
			}
		}
		current = current->next;
	}
	return (true);
}
