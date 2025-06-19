/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:54:37 by clu               #+#    #+#             */
/*   Updated: 2025/05/18 01:49:27 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Finalizes a command node with proper memory allocation
** Return AST node for the command.
*/
t_ast	*finalize_cmd_node(t_token **tokens, t_token *current,
						t_cmd *cmd, t_ms *ms)
{
	t_ast	*node;
	int		arg_count;

	arg_count = 0;
	while (cmd->full_cmd && cmd->full_cmd[arg_count])
		arg_count++;
	cmd->full_cmd = ft_realloc(cmd->full_cmd, sizeof(char *) * (arg_count),
			sizeof(char *) * (arg_count + 1));
	if (cmd->full_cmd)
		cmd->full_cmd[arg_count] = NULL;
	ms->cmd_no++;
	node = new_ast_node(NODE_COMMAND, NULL, NULL, cmd);
	*tokens = current;
	return (node);
}

/*
** Creates a missing command node when no command name is found
*/
t_ast	*create_missing_cmd_node(t_token **tokens, t_token *current,
									t_cmd *cmd, t_ms *ms)
{
	t_ast	*node;

	node = new_ast_node(NODE_MISSCMD, NULL, NULL, cmd);
	ms->cmd_no++;
	*tokens = current;
	return (node);
}
