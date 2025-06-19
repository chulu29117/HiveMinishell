/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:48:40 by mpierce           #+#    #+#             */
/*   Updated: 2025/04/28 14:26:18 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Checks if OLDPWD and PWD exists in envp
** Updates OLDPWD if they exist when using cd
*/
void	oldpwd(t_ms *ms)
{
	char	*oldpwd;
	char	*temp;

	temp = find_var(ms, ms->envp, "OLDPWD");
	if (!temp)
		return ;
	else
		temp = NULL;
	oldpwd = find_var(ms, ms->envp, "PWD");
	if (!oldpwd)
	{
		export_var_to_envp(ms, "OLDPWD");
		free(temp);
		return ;
	}
	temp = ft_strjoin("OLDPWD=", oldpwd);
	if (!temp)
		ms_error(ms, "ft_strjoin failure", 1, 0);
	export_var_to_envp(ms, temp);
	free(temp);
}
