/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:26:11 by clu               #+#    #+#             */
/*   Updated: 2025/05/17 19:50:36 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_token	*process_token(t_ms *ms, const char *input, int *i);
static t_token	*process_word_token(t_ms *ms, const char *input, int *i);
static t_token	*check_token_error(t_token *new_token);
static t_token	*token_error(t_token *new_token);

/*
** Tokenizes the given input string.
** Skips whitespace, calls process_token() to get the next token,
** check for token errors, and append tokens to the list.
*/
t_token	*lexer(t_ms *ms, const char *input)
{
	t_token	*token;
	t_token	*new_token;
	int		i;

	token = NULL;
	new_token = NULL;
	i = 0;
	while (input[i])
	{
		while (ft_is_whitespace(input[i]))
			i++;
		if (input[i] == '\0')
			break ;
		new_token = process_token(ms, input, &i);
		if (!check_token_error(new_token))
		{
			free_tokens(new_token);
			return (NULL);
		}
		append_token(&token, new_token);
	}
	return (token);
}

/*
** Determine the type of token based on the current char.
** or process_word_token accordingly.
*/
static t_token	*process_token(t_ms *ms, const char *input, int *i)
{
	t_token	*token;

	if (input[*i] == '|')
		token = handle_pipe(i);
	else if (input[*i] == '<' || input[*i] == '>')
		token = handle_redirection(input, i);
	else
		token = process_word_token(ms, input, i);
	return (token);
}

/*
** Processe a word token by reading a word.
** Handle quoted segments and variable segments.
** Build a list of segments and create a token.
** Create TOKEN_WORD token with the segments.
*/
static t_token	*process_word_token(t_ms *ms, const char *input, int *i)
{
	t_token		*token;
	t_segment	*segments;
	int			quoted_state;

	quoted_state = UNQUOTED;
	segments = read_word_seg(ms, input, i, &quoted_state);
	if (!segments)
		return (NULL);
	token = create_token(TOKEN_WORD, NULL, quoted_state);
	if (!token)
	{
		free_segments(segments);
		return (NULL);
	}
	token->segments = segments;
	return (token);
}

/*
** Token validation.
** Word tokens must have a value or segments.
** Other tokens must have a value.
*/
static t_token	*check_token_error(t_token *new_token)
{
	if (!new_token)
		return (token_error(new_token));
	if (new_token->type == TOKEN_WORD)
	{
		if (!new_token->value && !new_token->segments)
			return (token_error(new_token));
	}
	else if (!new_token->value)
		return (token_error(new_token));
	return (new_token);
}

/*
** Free token and print error message upon error.
*/
static t_token	*token_error(t_token *new_token)
{
	free_tokens(new_token);
	ft_putstr_fd("error: ", STDERR_FILENO);
	return (NULL);
}
