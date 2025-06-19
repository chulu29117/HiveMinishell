/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:46:17 by clu               #+#    #+#             */
/*   Updated: 2025/05/18 00:20:40 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_env_value(t_ms *ms, char *content);
static char	*expand_exit_stat(t_ms *ms);

/*
** Expand variable token by checking for '$' followed by a valid variable name.
** If the variable is followed by '?', it expands to the exit status.
** For other variables, it retrieves the value from the environment.
*/
char	*expand_var_token(t_ms *ms, char *content, int *pos)
{
	char	*var_token;
	char	*expanded_var;
	int		start;

	start = *pos;
	(*pos)++;
	if (content[*pos] == '?')
	{
		(*pos)++;
		return (expand_exit_stat(ms));
	}
	if (content[*pos] == '\0')
		return (x_strdup(ms, "$"));
	while (content[*pos] && (ft_isalnum(content[*pos]) || content[*pos] == '_'))
		(*pos)++;
	var_token = x_substr(ms, content, start + 1, *pos - start - 1);
	if (!var_token || !var_token[0])
	{
		free(var_token);
		return (ft_strdup("$"));
	}
	expanded_var = expand_env_value(ms, var_token);
	free(var_token);
	return (expanded_var);
}

/*
** Process a literal token.
** Read the parts without '$' and just return the literal string.
*/
char	*expand_literal_token(t_ms *ms, char *content, int *pos)
{
	int		start;
	char	*literal;

	start = *pos;
	while (content[*pos] && content[*pos] != '$')
		(*pos)++;
	literal = x_substr(ms, content, start, *pos - start);
	return (literal);
}

/*
** Expands an environment variable by finding its value in envp.
** Compares the variable name with envp entries and returns the value after '='.
** If not found, returns an empty string.
*/
static char	*expand_env_value(t_ms *ms, char *var)
{
	int		i;
	size_t	len;
	char	*result;

	i = 0;
	len = ft_strlen(var);
	while (ms->envp[i])
	{
		if (!ft_strncmp(ms->envp[i], var, len)
			&& (ms->envp[i][len] == '='))
		{
			result = x_strdup(ms, ms->envp[i] + len + 1);
			return (result);
		}
		i++;
	}
	result = x_strdup(ms, "");
	return (result);
}

/*
** Process '$?' to get the exit status.
** Converts the exit status to a string.
*/
static char	*expand_exit_stat(t_ms *ms)
{
	char	*exit_stat;

	exit_stat = ft_itoa(ms->exit_status);
	if (!exit_stat)
	{
		perror("ft_itoa failed");
		bi_exit(ms, 1, 1);
	}
	return (exit_stat);
}
