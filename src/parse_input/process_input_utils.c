/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:03:43 by clu               #+#    #+#             */
/*   Updated: 2025/05/19 00:42:59 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Process the input string and tokenize it.
** Add the input to the history.
*/
bool	process_tokens(t_ms *ms, char *input)
{
	t_token	*token;

	add_history(input);
	token = lexer(ms, input);
	if (!token)
	{
		ft_putstr_fd("tokenization error\n", STDERR_FILENO);
		free(input);
		return (false);
	}
	ms->token = token;
	free(input);
	return (true);
}
