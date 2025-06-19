/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:00:11 by mpierce           #+#    #+#             */
/*   Updated: 2025/04/28 14:19:32 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Joins the prompt with cwd to the ending $
** Resets colour code
*/
char	*create_prompt(t_ms *ms, char *prompt)
{
	char	*temp;

	temp = ft_strjoin(prompt, PROMPT_END);
	if (!temp)
	{
		free(prompt);
		ms_error(ms, "prompt failure", 1, 0);
	}
	free(prompt);
	return (temp);
}

/*
** Calls ft_strjoin() and handles error
*/
char	*j_strjoin(t_ms *ms, char *s1, char *s2)
{
	char	*temp;

	temp = ft_strjoin(s1, s2);
	if (!temp)
		ms_error(ms, "ft_strjoin failure", 1, 0);
	return (temp);
}

/*
** Compares cwd with the HOME variable
** If cwd starts with HOME it is replaces with ~
** If HOME is unset full path is used
*/
char	*compare_path(t_ms *ms, char *home, char *cwd)
{
	int		i;
	char	*temp;
	char	*temp2;

	i = -1;
	while (home[++i])
	{
		if (home[i] != cwd[i] || !cwd[i])
			return (j_strjoin(ms, cwd, PROMPT_END));
	}
	temp = ft_strjoin("~", cwd + i);
	if (!temp)
		ms_error(ms, "prompt failure", 1, 0);
	temp2 = create_prompt(ms, temp);
	return (temp2);
}

/*
** Checks envp for PWD
** If PWD unset then getcwd() called
** If getcwd() fails then backup of PWD is used
** Sets alloc bool to indicate if pwd is allocated
*/
char	*get_pwd(t_ms *ms, bool *alloc)
{
	char	*pwd;

	pwd = find_var(ms, ms->envp, "PWD");
	if (!pwd)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			if (errno == ENOENT)
			{
				*alloc = false;
				return (".");
			}
			else
				ms_error(ms, "getcwd failure", 1, 0);
		}
		*alloc = true;
	}
	return (pwd);
}

/*
** Creates the prompt from the cwd
** Appends shell name to the start and $ to the end
** Colour code is bold cyan
*/
void	get_prompt(t_ms *ms)
{
	char	*pwd;
	char	*home;
	bool	alloc;
	char	*temp;

	alloc = false;
	pwd = get_pwd(ms, &alloc);
	home = find_var(ms, ms->envp, "HOME");
	if (ms->prompt)
		free(ms->prompt);
	if (!home)
	{
		ms->prompt = create_prompt(ms, j_strjoin(ms, PROMPT_START, pwd));
		if (alloc)
			free(pwd);
		return ;
	}
	temp = compare_path(ms, home, pwd);
	ms->prompt = ft_strjoin(PROMPT_START, temp);
	free(temp);
	if (alloc == true)
		free(pwd);
	if (!ms->prompt)
		ms_error(ms, "prompt failure", 1, 0);
}
