/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:17:52 by mpierce           #+#    #+#             */
/*   Updated: 2025/04/22 17:41:31 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free_infile(t_infile **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		if (array[i])
		{
			free(array[i]->name);
			free(array[i]);
		}
		i++;
	}
	free(array);
}

/*
** Frees all file arrays 
*/
void	free_cmd_help(t_cmd *cmd)
{
	if (cmd->full_cmd)
	{
		ft_free_array(cmd->full_cmd);
		cmd->full_cmd = NULL;
	}
	if (cmd->infile)
	{
		ft_free_infile(cmd->infile);
		cmd->infile = NULL;
	}
	if (cmd->outfile)
	{
		ft_free_array(cmd->outfile);
		cmd->outfile = NULL;
	}
	if (cmd->append)
	{
		free(cmd->append);
		cmd->append = NULL;
	}
}

/*
** Frees cmd array
*/
void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_cmd_help(cmd);
	free(cmd);
}
