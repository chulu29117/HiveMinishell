/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:56:49 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/23 10:40:10 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	run_export_args(t_ms *ms, char **full_cmd)
{
	int	i;

	i = 0;
	if (!full_cmd[1])
	{
		export_var_to_envp(ms, NULL);
		return ;
	}
	while (full_cmd[++i])
	{
		if (export_var_to_envp(ms, full_cmd[i]) == false)
			ms->exit_status = 1;
	}
}

/*
** Uses bubble sort to sort envp to alphabetical order
*/
void	sort_env(char **temp)
{
	int		i;
	char	*holder;
	int		sort;

	while (1)
	{
		i = -1;
		sort = 0;
		while (temp[++i] && temp[i + 1])
		{
			if (ft_strcmp(temp[i], temp[i + 1]) > 0)
			{
				holder = temp[i];
				temp[i] = temp[i + 1];
				temp[i + 1] = holder;
				sort = 1;
			}
		}
		if (sort == 0)
			break ;
	}
}

/*
** Creates a temporary array to write envp in order
*/
void	export_print_env(t_ms *ms)
{
	char	**temp;
	int		i;

	i = -1;
	temp = malloc((ms->elements + 1) * sizeof(char *));
	if (!temp)
		ms_error(ms, "envp allocation failed", 1, 0);
	while (++i < ms->elements)
		temp[i] = ms->envp[i];
	temp[i] = NULL;
	sort_env(temp);
	i = -1;
	while (temp[++i])
	{
		if (!(temp[i][0] == '_' && temp[i][1] == '='))
			printf("declare -x %s\n", temp[i]);
	}
	free(temp);
}

/*
** Checks that identifier contains only alphanumerical characters
*/
bool	check_valid_identifier(t_ms *ms, char *name)
{
	int	i;

	i = -1;
	if (!name)
		ms_error(ms, "Export identifier failure", 1, 0);
	if (name[0] == 0)
		return (false);
	if ((name[0] > 47 && name[0] < 58) || ((name[0] < 65 || name[0] > 90)
			&& (name[0] < 97 || name[0] > 122) && name[0] != 95))
		return (false);
	while (name[++i])
	{
		if ((name[i] < 65 || name[i] > 90) && (name[i] < 97
				|| name[i] > 122) && name[i] != 95
			&& (name[i] < 47 || name[i] > 58))
			return (false);
	}
	return (true);
}

/*
** Searches for variable in envp when appending
** If variable exists it will be modified
** If it doesn't exist it will be created
*/
char	*append_var(t_ms *ms, char *var, char *name, int j)
{
	char	*temp1;
	char	*temp2;
	char	*temp3;

	temp1 = find_var(ms, ms->envp, name);
	while (var[++j] != '=' && var[j])
		;
	if (temp1)
	{
		temp2 = ft_strjoin(temp1, var + j + 1);
		if (!temp2)
			ms_error(ms, "ft_strjoin failed", 1, 0);
	}
	else
		temp2 = ft_strdup(var + j + 1);
	temp1 = ft_strjoin(name, "=");
	if (!temp1)
		ms_error(ms, "ft_strjoin failed", 1, 0);
	temp3 = ft_strjoin(temp1, temp2);
	if (!temp3)
		ms_error(ms, "ft_strjoin failed", 1, 0);
	free(temp1);
	free(temp2);
	return (temp3);
}
