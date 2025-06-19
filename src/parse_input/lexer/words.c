/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:29:19 by clu               #+#    #+#             */
/*   Updated: 2025/05/18 00:18:33 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Process segments of a word until operator or end of string.
** Handle quotes, variable segments, and literal segments.
** Build a list of segments and set the quoted state in the word struct.
*/
t_segment	*read_word_seg(t_ms *ms, const char *input, int *i,
	int *quoted_state)
{
	t_word	word;

	word.has_quote = 0;
	word.quoted_state = UNQUOTED;
	word.segments = NULL;
	while (input[*i] && !is_operator(input[*i]))
	{
		if (input[*i] == '\'')
			handle_s_quote(ms, input, i, &word);
		else if (input[*i] == '"')
			handle_d_quote(ms, input, i, &word);
		else if (input[*i] == '$')
			handle_var_seg(ms, input, i, &word);
		else
			handle_lit_seg(ms, input, i, &word);
	}
	if (!word.has_quote)
		*quoted_state = UNQUOTED;
	else
		*quoted_state = word.quoted_state;
	return (word.segments);
}
