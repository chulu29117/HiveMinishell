/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extras.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 16:28:40 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/20 15:43:37 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	invalid_export(t_ms *ms, char *name)
{
	char	*temp1;
	char	*temp2;

	temp1 = ft_strjoin("minishell: export: `", name);
	if (!temp1)
		ms_error(ms, "ft_strjoin failure", 1, 0);
	temp2 = ft_strjoin(temp1, "': is not a valid identifier\n");
	if (!temp2)
	{
		free(temp1);
		ms_error(ms, "ft_strjoin failure", 1, 0);
	}
	write(2, temp2, ft_strlen(temp2));
	free(temp1);
	free(temp2);
	ms->exit_status = 1;
}
