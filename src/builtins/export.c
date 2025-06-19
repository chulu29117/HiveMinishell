/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:15:51 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/20 15:42:45 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Frees the memory in the envp
** Frees the envp pointer itself
** Sets envp pointer to null
*/
void	free_envp(t_ms *ms)
{
	int	i;

	i = -1;
	if (ms->envp)
	{
		while (ms->envp[++i])
			free(ms->envp[i]);
		free(ms->envp);
		ms->envp = NULL;
	}
}

/*
** Creates new envp array
** Replaces matching variable with new value
** Frees old envp array
*/
void	overwrite_var(t_ms *ms, char *var, char *name)
{
	char	**tempenvp;
	int		i;
	int		j;

	i = -1;
	j = find_in_envp(ms, ms->envp, name);
	free(name);
	tempenvp = malloc((ms->elements + 1) * sizeof(char *));
	if (!tempenvp)
		ms_error(ms, "envp allocation failure", 1, 0);
	while (++i < ms->elements)
	{
		if (i == j)
		{
			tempenvp[i] = ft_strdup(var);
			if (!tempenvp[i])
				arr_dup_fail(ms, tempenvp, i);
		}
		else
			dup_envp(ms, tempenvp, i, i);
	}
	tempenvp[i] = NULL;
	free_envp(ms);
	ms->envp = tempenvp;
}

/*
** Creates new envp array
** Appends new variable to the end of the new envp
** Frees old envp array
*/
void	create_var(t_ms *ms, char *var)
{
	char	**tempenvp;
	int		i;

	i = -1;
	tempenvp = malloc((ms->elements + 2) * sizeof(char *));
	if (!tempenvp)
		ms_error(ms, "envp allocation failure", 1, 0);
	while (++i < ms->elements - 1)
		dup_envp(ms, tempenvp, i, i);
	tempenvp[i] = ft_strdup(var);
	if (!tempenvp[i])
		arr_dup_fail(ms, tempenvp, i);
	i++;
	tempenvp[i] = ft_strdup(ms->envp[i - 1]);
	tempenvp[++i] = NULL;
	free_envp(ms);
	ms->elements++;
	ms->envp = tempenvp;
}

/*
** Controls export whether it needs to update or create variable
*/
void	run_export(t_ms *ms, char *var, char *name, bool append)
{
	char	*temp;

	if (append == true)
		temp = append_var(ms, var, name, -1);
	else
		temp = ft_strdup(var);
	if (!temp)
		ms_error(ms, "export allocation failure", 1, 0);
	if (find_var(ms, ms->envp, name))
	{
		overwrite_var(ms, temp, name);
	}
	else
	{
		free(name);
		create_var(ms, temp);
	}
	free(temp);
}

/*
** Checks if a variable has been given to export
** Protects the _ variable 
** Detects if export is to append variable
** Sets exit code to 0
*/
bool	export_var_to_envp(t_ms *ms, char *var)
{
	int		i;
	char	*name;
	bool	append;

	i = -1;
	append = false;
	if (!var)
	{
		export_print_env(ms);
		return (true);
	}
	if (var[0] == '_' && (var[1] == 0 || var[1] == '='))
		return (true);
	while (var[++i] != '=' && var[i])
		if (var[i + 1] && var[i] == '+' && var[i + 1] == '=')
			append = true;
	name = x_substr(ms, var, 0, i - append);
	if (check_valid_identifier(ms, name) == false)
	{
		invalid_export(ms, name);
		free(name);
		return (false);
	}
	run_export(ms, var, name, append);
	return (true);
}
