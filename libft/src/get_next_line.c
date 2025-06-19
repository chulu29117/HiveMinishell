/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:48:49 by clu               #+#    #+#             */
/*   Updated: 2025/01/15 16:29:05 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*allocate_buffer(char **prev_buffer)
{
	char	*temp_buffer;

	temp_buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (temp_buffer == NULL)
		return (NULL);
	if (*prev_buffer == NULL)
	{
		*prev_buffer = ft_strdup("");
		if (*prev_buffer == NULL)
		{
			free(temp_buffer);
			return (NULL);
		}
	}
	return (temp_buffer);
}

static char	*read_file_fill_buffer(int fd, char *prev_buffer)
{
	char	*temp_buffer;
	char	*temp;
	ssize_t	bytes_read;

	temp_buffer = allocate_buffer(&prev_buffer);
	if (temp_buffer == NULL)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(prev_buffer, '\n') && bytes_read != 0)
	{
		bytes_read = read(fd, temp_buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(temp_buffer), free(prev_buffer), NULL);
		temp_buffer[bytes_read] = '\0';
		temp = ft_strjoin(prev_buffer, temp_buffer);
		if (temp == NULL)
			return (free(temp_buffer), free(prev_buffer), NULL);
		free(prev_buffer);
		prev_buffer = temp;
	}
	free(temp_buffer);
	return (prev_buffer);
}

static char	*set_line(char *prev_buffer)
{
	int		i;
	char	*line;

	if (prev_buffer == NULL)
		return (NULL);
	i = 0;
	while (prev_buffer[i] && prev_buffer[i] != '\n')
		i++;
	if (prev_buffer[i] == '\n')
		i++;
	line = ft_substr(prev_buffer, 0, i);
	if (line == NULL)
		return (NULL);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*line_buffer[MAX_FD];
	char		*line;
	char		*temp;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	line_buffer[fd] = read_file_fill_buffer(fd, line_buffer[fd]);
	if (line_buffer[fd] == NULL || line_buffer[fd][0] == '\0')
		return (free(line_buffer[fd]), line_buffer[fd] = NULL, NULL);
	line = set_line(line_buffer[fd]);
	if (line == NULL)
		return (free(line_buffer[fd]), line_buffer[fd] = NULL, NULL);
	temp = ft_strdup(line_buffer[fd] + ft_strlen(line));
	if (temp == NULL)
		return (free(line_buffer[fd]), line_buffer[fd] = NULL, NULL);
	free(line_buffer[fd]);
	line_buffer[fd] = temp;
	return (line);
}
