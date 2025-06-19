/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:57:53 by clu               #+#    #+#             */
/*   Updated: 2025/05/18 01:53:43 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Creates a new AST node with the given type, left and right nodes, cmd data.
*/
t_ast	*new_ast_node(t_node_type type, t_ast *left, t_ast *right, t_cmd *cmd)
{
	t_ast	*ast;

	ast = (t_ast *)malloc(sizeof(t_ast));
	if (!ast)
	{
		free_cmd(cmd);
		free_ast(ast);
		error(NULL, "malloc failed");
	}
	ast->type = type;
	ast->left = left;
	ast->right = right;
	ast->cmd = cmd;
	return (ast);
}

/*
** Creates a new command structure.
*/
t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		error(NULL, "malloc failed in new_cmd");
	cmd->full_cmd = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = NULL;
	cmd->from_expand = false;
	return (cmd);
}
