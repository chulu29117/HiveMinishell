/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:50:16 by clu               #+#    #+#             */
/*   Updated: 2025/05/05 10:24:09 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Frees the ms struct and all its allocated fields.
*/
void	free_ms(t_ms *ms)
{
	int	i;

	if (!ms)
		return ;
	if (ms->envp)
	{
		i = 0;
		while (ms->envp[i])
		{
			free(ms->envp[i]);
			i++;
		}
		free(ms->envp);
	}
	free_structs(ms);
	free(ms);
}

void	free_structs(t_ms *ms)
{
	if (ms->token)
	{
		free_tokens(ms->token);
		ms->token = NULL;
	}
	if (ms->word)
	{
		free_word(ms->word);
		ms->word = NULL;
	}
	if (ms->ast)
	{
		free_ast(ms->ast);
		ms->ast = NULL;
		ms->cmd = NULL;
	}
	clean_all_heredocs(ms);
	ms->heredoc_no = 0;
}

void	free_segments(t_segment *segments)
{
	t_segment	*current;
	t_segment	*next;

	current = segments;
	while (current)
	{
		next = current->next;
		if (current->seg)
		{
			free(current->seg);
			current->seg = NULL;
		}
		free(current);
		current = next;
	}
}

void	free_word(t_word *word)
{
	if (!word)
		return ;
	free_segments(word->segments);
	word->segments = NULL;
	free(word);
}

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	free_ast(ast->left);
	free_ast(ast->right);
	if (ast->cmd)
		free_cmd(ast->cmd);
	free(ast);
}
