/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:12:22 by clu               #+#    #+#             */
/*   Updated: 2025/05/19 00:29:24 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Check if the next token is a word token for syntax validation.
** Handle the redirection based on its type.
*/
bool	parse_redirect(t_ms *ms, t_token **tokens, t_cmd *cmd)
{
	t_token	*current;

	current = *tokens;
	if (!current || !current->next || current->next->type != TOKEN_WORD)
	{
		ms->syntax_error = true;
		return (false);
	}
	if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_APPEND)
		handle_out_redirect(current, cmd);
	else if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
	{
		if (handle_in_redirect(ms, current, cmd) == false)
			return (false);
	}
	else
		error(NULL, "Invalid redirection type");
	*tokens = current->next->next;
	return (true);
}

/*
** Add the target file to the outfile list in the cmd struct.
** Check and set the append flag.
*/
void	handle_out_redirect(t_token *token, t_cmd *cmd)
{
	if (ft_add_to_array(&cmd->outfile, token->next->value) == -1)
		error(NULL, "malloc failed in parse_redirect");
	cmd->append = ft_realloc(cmd->append,
			sizeof(int) * (ft_array_length(cmd->outfile) - 1),
			sizeof(int) * ft_array_length(cmd->outfile));
	if (!cmd->append)
		error(NULL, "malloc failed in parse_redirect");
	if (token->type == TOKEN_APPEND)
		cmd->append[ft_array_length(cmd->outfile) - 1] = 1;
	else
		cmd->append[ft_array_length(cmd->outfile) - 1] = 0;
}

/*
** Create the infile struct.
** Set the infile name and heredoc flag.
*/
t_infile	*new_infile(char *name, int is_heredoc)
{
	t_infile	*infile;

	infile = malloc(sizeof(t_infile));
	if (!infile)
		error(NULL, "malloc failed in new_infile");
	infile->name = ft_strdup(name);
	if (!infile->name)
	{
		free(infile->name);
		free(infile);
		error(NULL, "malloc failed in new_infile");
	}
	infile->heredoc = is_heredoc;
	return (infile);
}

/*
** Add the infile to the infile struct.
*/
bool	ft_add_to_infile(t_infile ***array, t_infile *infile)
{
	int			count;
	t_infile	**new_array;

	count = ft_array_length((char **)*array);
	new_array = ft_realloc(*array, sizeof(t_infile *) * (count + 1),
			sizeof(t_infile *) * (count + 2));
	if (!new_array)
		return (false);
	new_array[count] = infile;
	if (!new_array[count])
	{
		free(new_array);
		return (false);
	}
	new_array[count + 1] = NULL;
	*array = new_array;
	return (true);
}

/*
** For heredoc tokens, set up the heredoc and add it to the infile list.
** For other tokens, create a new infile struct and add it to the list.
*/
bool	handle_in_redirect(t_ms *ms, t_token *token, t_cmd *cmd)
{
	t_infile	*infile;
	int			is_heredoc;

	if (token->type == TOKEN_HEREDOC)
	{
		if (!setup_heredoc(ms, token, &infile))
			return (false);
	}
	else
	{
		is_heredoc = 0;
		infile = new_infile(token->next->value, is_heredoc);
	}
	if (ft_add_to_infile(&cmd->infile, infile) == false)
	{
		free(infile->name);
		free(infile);
		error(NULL, "malloc failed in handle_in_redirect");
	}
	return (true);
}
