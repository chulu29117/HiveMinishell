/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:53:27 by mpierce           #+#    #+#             */
/*   Updated: 2025/05/13 12:11:49 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Writes both variables into one error message
** Always returns -1 to indicate error
*/
int	print_error(t_ms *ms, char *name, char *msg)
{
	char	*temp;

	temp = ft_strjoin(name, msg);
	if (!temp)
		ms_error(ms, "ft_strjoin failure", 1, 0);
	ft_putendl_fd(temp, 2);
	free(temp);
	ms->exit_status = 1;
	return (-1);
}

/*
** Finds variable specified in name in the envp
** Returns pointer to the value of variable
*/
char	*find_var(t_ms *ms, char **envp, char *name)
{
	int		i;
	int		j;
	char	*path;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		path = x_substr(ms, envp[i], 0, j);
		if (!ft_strcmp(path, name))
		{
			free(path);
			return (envp[i] + j + 1);
		}
		free(path);
		i++;
	}
	return (NULL);
}

/*
** Finds variable specified in var in the envp
** Returns index in the envp for the variable
*/
int	find_in_envp(t_ms *ms, char **envp, char *var)
{
	int		i;
	int		j;
	char	*path;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		path = x_substr(ms, envp[i], 0, j);
		if (!ft_strcmp(path, var))
		{
			free(path);
			return (i);
		}
		free(path);
		i++;
	}
	return (-1);
}

/*
** Duplicates a single line of the envp
** In case of error, will free all previously allocated lines
*/
void	dup_envp(t_ms *ms, char **temp, int i, int j)
{
	temp[j] = ft_strdup(ms->envp[i]);
	if (!temp[j])
		arr_dup_fail(ms, temp, j);
}

/*
** Frees all allocated memory in an array
** Free the array pointer itself and exits
*/
void	arr_dup_fail(t_ms *ms, char **arr, int j)
{
	while (j--)
		free(arr[j]);
	free(arr);
	ms_error(ms, "Array duplication failed", 1, 0);
}
