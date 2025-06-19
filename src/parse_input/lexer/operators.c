/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:31:18 by clu               #+#    #+#             */
/*   Updated: 2025/05/17 18:25:06 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_token	*handle_redirection_in(const char *input, int *i);
static t_token	*handle_redirection_out(const char *input, int *i);

/*
** Creates and returns a token for a pipe ('|') operator.
** Sets the token type to TOKEN_PIPE and the quoted state to UNQUOTED.
*/
t_token	*handle_pipe(int *i)
{
	t_token	*token;

	token = create_token(TOKEN_PIPE, ft_strdup("|"), UNQUOTED);
	if (!token)
		return (NULL);
	(*i)++;
	return (token);
}

/*
** Handles redirection operators ('<' or '>').
*/
t_token	*handle_redirection(const char *input, int *i)
{
	if (input[*i] == '<')
		return (handle_redirection_in(input, i));
	else
		return (handle_redirection_out(input, i));
}

/*
** Process input redirection.
** If the next character is also '<', creates a heredoc token ("<<");
** Else, creates a single input redirection token ("<").
*/
static t_token	*handle_redirection_in(const char *input, int *i)
{
	t_token	*token;

	(*i)++;
	if (input[*i] == '<')
	{
		token = create_token(TOKEN_HEREDOC, ft_strdup("<<"), UNQUOTED);
		if (!token)
			return (NULL);
		(*i)++;
	}
	else
	{
		token = create_token(TOKEN_REDIR_IN, ft_strdup("<"), UNQUOTED);
		if (!token)
			return (NULL);
	}
	return (token);
}

/*
** Process output redirection.
** If the next character is also '>', creates an append token (">>");
** Else, creates a single output redirection token (">").
*/
static t_token	*handle_redirection_out(const char *input, int *i)
{
	t_token	*token;

	(*i)++;
	if (input[*i] == '>')
	{
		token = create_token(TOKEN_APPEND, ft_strdup(">>"), UNQUOTED);
		if (!token)
			return (NULL);
		(*i)++;
	}
	else
	{
		token = create_token(TOKEN_REDIR_OUT, ft_strdup(">"), UNQUOTED);
		if (!token)
			return (NULL);
	}
	return (token);
}

/*
** Checks if the character is a operator.
*/
bool	is_operator(char c)
{
	return (ft_is_whitespace(c) || c == '|' || c == '<' || c == '>'
		|| c == '\0');
}
