/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retoken.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:12:08 by clu               #+#    #+#             */
/*   Updated: 2025/05/20 21:17:33 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	split_token_with_spaces(t_ms *ms, t_token *current);
static void	split_expanded_token(t_ms *ms, t_token *current);
static void	split_tokens(t_ms *ms, t_token *current, char **token_split);

/*
** Retokenize the token list.
** Split tokens with spaces into multiple tokens.
** Or split expand tokens with '$' into multiple tokens.
*/
void	retokenize(t_ms *ms)
{
	t_token	*current;
	t_token	*next;

	current = ms->token;
	while (current)
	{
		next = current->next;
		if (current->type == TOKEN_WORD && ft_strchr(current->value, ' ')
			&& current->quoted == UNQUOTED)
			split_token_with_spaces(ms, current);
		else if (current->type == TOKEN_WORD && current->prev == NULL
			&& ft_strchr(current->value, '$'))
			split_expanded_token(ms, current);
		current = next;
	}
}

/*
** Split a token with spaces into multiple tokens.
*/
static void	split_token_with_spaces(t_ms *ms, t_token *current)
{
	char	**token_split;

	token_split = ft_split(current->value, ' ');
	if (token_split && token_split[0])
	{
		free(current->value);
		current->value = x_strdup(ms, token_split[0]);
		split_tokens(ms, current, token_split);
		ft_free_array(token_split);
	}
}

/*
** Split an expanded token with '$' into multiple tokens.
*/
static void	split_expanded_token(t_ms *ms, t_token *current)
{
	char	**token_split;

	token_split = ft_split(current->value, ' ');
	if (token_split && token_split[0])
	{
		free(current->value);
		current->value = x_strdup(ms, token_split[0]);
		split_tokens(ms, current, token_split);
		ft_free_array(token_split);
	}
}

/*
** Split the token into multiple tokens.
** The first token is kept as is, and the rest are added as new tokens.
*/
static void	split_tokens(t_ms *ms, t_token *current, char **token_split)
{
	t_token	*prev_token;
	t_token	*new_token;
	int		i;

	i = 1;
	prev_token = current;
	while (token_split[i])
	{
		new_token = create_token(TOKEN_WORD,
				x_strdup(ms, token_split[i]), UNQUOTED);
		new_token->next = prev_token->next;
		new_token->prev = prev_token;
		if (prev_token->next)
			prev_token->next->prev = new_token;
		prev_token->next = new_token;
		prev_token = new_token;
		i++;
	}
}
