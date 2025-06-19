/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:57:28 by clu               #+#    #+#             */
/*   Updated: 2025/05/17 18:03:36 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Allocates memory for a new token.
** Doubly-linked list structure.
*/
t_token	*allocate_token(void)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = 0;
	token->value = NULL;
	token->next = NULL;
	token->prev = NULL;
	token->quoted = UNQUOTED;
	token->segments = NULL;
	return (token);
}

/*
** Creates a new token.
** Sets the type, value, and quoted state.
** Initialize segments to NULL and expanded flag to false.
*/
t_token	*create_token(t_token_type type, char *value, int quoted)
{
	t_token	*token;

	token = allocate_token();
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->quoted = quoted;
	token->segments = NULL;
	token->expanded = false;
	return (token);
}

/*
** Appends a new token to the end of the list.
** Updates the prev pointer of the new token.
*/
void	append_token(t_token **tokens, t_token *new_token)
{
	t_token	*last;

	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	last = *tokens;
	while (last->next)
		last = last->next;
	last->next = new_token;
	new_token->prev = last;
}

/*
** Frees the memory allocated for a token.
*/
void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
	{
		free(token->value);
		token->value = NULL;
	}
	if (token->segments)
	{
		free_segments(token->segments);
		token->segments = NULL;
	}
}

/*
** Frees the memory allocated for a list of tokens.
*/
void	free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		free_token(tokens);
		free(tokens);
		tokens = next;
	}
}
