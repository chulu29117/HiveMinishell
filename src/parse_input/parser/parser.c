/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:57:40 by clu               #+#    #+#             */
/*   Updated: 2025/05/18 01:42:54 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Parses the input tokens into an abstract syntax tree (AST).
** The AST is built, each node representing a command or a pipe.
*/
t_ast	*parse_input(t_token **tokens, t_ms *ms)
{
	t_ast	*left_ast;
	t_ast	*right_ast;
	t_token	*current;

	if (!validate_input(ms, *tokens))
		return (NULL);
	current = *tokens;
	left_ast = parse_cmd(&current, ms);
	if (current && current->type == TOKEN_PIPE)
	{
		current = current->next;
		right_ast = parse_input(&current, ms);
		if (!right_ast)
		{
			free_ast(left_ast);
			return (NULL);
		}
		left_ast = new_ast_node(NODE_PIPE, left_ast, right_ast, NULL);
	}
	return (left_ast);
}

/*
** Parses a command with its arguments and redirections.
** Process the leading redirections, command name, then the arguments.
** If the command is missing, create a node with NULL command.
*/
t_ast	*parse_cmd(t_token **tokens, t_ms *ms)
{
	t_cmd	*cmd;
	t_token	*current;
	int		arg_count;
	t_ast	*node;

	node = NULL;
	arg_count = 0;
	cmd = new_cmd();
	current = *tokens;
	if (lead_redirect(&current, cmd, ms))
	{
		if (!process_cmd_name(&current, cmd, &arg_count, ms))
			node = create_missing_cmd_node(tokens, current, cmd, ms);
		else if (process_cmd_args(&current, cmd, &arg_count, ms))
			node = finalize_cmd_node(tokens, current, cmd, ms);
	}
	if (!node)
	{
		free_cmd(cmd);
		node = NULL;
	}
	return (node);
}

/*
** Process the leading redirections.
*/
bool	lead_redirect(t_token **current, t_cmd *cmd, t_ms *ms)
{
	while (*current && ((*current)->type == TOKEN_REDIR_IN
			|| (*current)->type == TOKEN_REDIR_OUT
			|| (*current)->type == TOKEN_APPEND
			|| (*current)->type == TOKEN_HEREDOC))
	{
		if (!parse_redirect(ms, &*current, cmd))
			return (false);
	}
	return (true);
}

/*
** Set the first element as the command name.
** Update the argument count.
** Flag whether the empty cmd came from an expanded token.
*/
bool	process_cmd_name(t_token **current, t_cmd *cmd, int *arg, t_ms *ms)
{
	t_token	*prev_token;

	if (*current && (*current)->type == TOKEN_WORD)
	{
		prev_token = *current;
		cmd->full_cmd = malloc(2 * sizeof(char *));
		if (!cmd->full_cmd)
			error(NULL, "malloc failed in parse_cmd");
		cmd->full_cmd[0] = x_strdup(ms, (*current)->value);
		cmd->full_cmd[1] = NULL;
		(*arg) = 1;
		*current = (*current)->next;
		if (prev_token->value && prev_token->value[0] == '\0'
			&& prev_token->expanded)
			cmd->from_expand = true;
	}
	else
		return (false);
	return (true);
}

/*
** For word tokens, add them to the command's full_cmd array.
** For redirection tokens, call parse_redirect to handle them.
*/
bool	process_cmd_args(t_token **current, t_cmd *cmd, int *arg, t_ms *ms)
{
	while (*current && ((*current)->type == TOKEN_WORD
			|| (*current)->type == TOKEN_REDIR_IN
			|| (*current)->type == TOKEN_REDIR_OUT
			|| (*current)->type == TOKEN_APPEND
			|| (*current)->type == TOKEN_HEREDOC))
	{
		if (*current && (*current)->type == TOKEN_WORD)
		{
			cmd->full_cmd = ft_realloc(cmd->full_cmd, sizeof(char *) * (*arg),
					sizeof(char *) * (*arg + 2));
			if (!cmd->full_cmd)
				error(NULL, "ft_realloc failed in parse_cmd");
			cmd->full_cmd[*arg] = x_strdup(ms, (*current)->value);
			(*arg)++;
			*current = (*current)->next;
		}
		else
		{
			if (!parse_redirect(ms, &*current, cmd))
				return (false);
		}
		if (cmd->full_cmd)
			cmd->full_cmd[*arg] = NULL;
	}
	return (true);
}
