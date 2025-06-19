/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:30:50 by clu               #+#    #+#             */
/*   Updated: 2025/05/17 18:45:08 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	add_segment(t_ms *ms, t_word *word, char *seg, int quoted);

/*
** Process a single-quoted segment.
** Substr the input string from the start of the quote to the end of the quote.
** Add the segment to the word struct with its single quote state.
*/
void	handle_s_quote(t_ms *ms, const char *input, int *i, t_word *word)
{
	int		start;
	char	*seg;

	word->has_quote = 1;
	word->quoted_state = SINGLE_QUOTED;
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	seg = x_substr(ms, input, start, *i - start);
	add_segment(ms, word, seg, SINGLE_QUOTED);
	if (input[*i] == '\'')
		(*i)++;
}

/*
** Process a double-quoted segment.
** Substr the input string from the start of the quote to the end of the quote.
** Add the segment to the word struct with its double quote state.
*/
void	handle_d_quote(t_ms *ms, const char *input, int *i, t_word *word)
{
	int		start;
	char	*seg;

	word->has_quote = 1;
	word->quoted_state = DOUBLE_QUOTED;
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '"')
		(*i)++;
	seg = x_substr(ms, input, start, *i - start);
	add_segment(ms, word, seg, DOUBLE_QUOTED);
	if (input[*i] == '"')
		(*i)++;
}

/*
** Process a variable segment.
** If followed by a quote, no variable expansion is done.
** If followed by a '?', the exit status is added to the segment.
** If followed by anything else, identify the variable name.
*/
void	handle_var_seg(t_ms *ms, const char *input, int *i, t_word *word)
{
	int		start;
	char	*seg;

	start = *i;
	(*i)++;
	if (input[*i] == '"' || input[*i] == '\'')
		return ;
	if (input[*i] == '?')
		(*i)++;
	else
	{
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;
	}
	seg = x_substr(ms, input, start, *i - start);
	add_segment(ms, word, seg, UNQUOTED);
}

/*
** Process a literal segment.
** Read until a quote, dollar sign, or operator is found.
*/
void	handle_lit_seg(t_ms *ms, const char *input, int *i, t_word *word)
{
	int		start;
	char	*seg;

	start = *i;
	while (input[*i] && input[*i] != '\'' && input[*i] != '"'
		&& input[*i] != '$' && !is_operator(input[*i]))
		(*i)++;
	seg = x_substr(ms, input, start, *i - start);
	add_segment(ms, word, seg, UNQUOTED);
}

/* 
** Add a new segment to the word struct.
** Memory for segment struct.
** Duplicate the segment string and set the quoted state.
** If the word struct has no segments, set the new segment as the first.
** Else, traverse the list to find the last segment and add the new one.
*/
static void	add_segment(t_ms *ms, t_word *word, char *seg, int quoted)
{
	t_segment	*segments;
	t_segment	*current;

	segments = (t_segment *)malloc(sizeof(t_segment));
	if (!segments)
	{
		perror("add_segment failed");
		bi_exit(ms, 1, 1);
	}
	segments->seg = x_strdup(ms, seg);
	free(seg);
	segments->quoted = quoted;
	segments->next = NULL;
	if (!word->segments)
		word->segments = segments;
	else
	{
		current = word->segments;
		while (current->next)
			current = current->next;
		current->next = segments;
	}
}
