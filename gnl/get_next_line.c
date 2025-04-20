/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:35:39 by jsayerza          #+#    #+#             */
/*   Updated: 2024/10/17 20:47:56 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*next_line(char *stash)
{
	char	*leftstr;
	int		i;
	int		j;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == 0)
	{
		free(stash);
		return (NULL);
	}
	leftstr = (char *)malloc(ft_strlen(stash) - i + 1);
	if (!leftstr)
		return (NULL);
	i++;
	j = 0;
	while (stash[i])
		leftstr[j++] = stash[i++];
	leftstr[j] = 0;
	free(stash);
	return (leftstr);
}

char	*set_line(char *stash)
{
	char	*newline;
	int		i;

	i = 0;
	if (stash[i] == 0)
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	newline = (char *)malloc(i + 2);
	if (!newline)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
		newline[i] = stash[i];
		i++;
	}
	if (stash[i] && stash[i] == '\n')
		newline[i++] = '\n';
	newline[i] = '\0';
	return (newline);
}

char	*fill_line_buffer(int fd, char *stash)
{
	char	*buffer;
	int		count;

	// printf("IN fill_line_buffer\n");
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	count = 1;
	// printf(" fill_line_buffer-1 Stash:%s Buffer:%s count:%d\n", stash, buffer, count);
	while (count > 0 && !(ft_strchr(stash, '\n')))
	{
		// printf(" fill_line_buffer- Buffer:%s\n", buffer);
		count = read(fd, buffer, BUFFER_SIZE);
		// printf(" fill_line_buffer-2\n");
		if (count == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[count] = 0;
		// printf(" fill_line_buffer-3\n");
		// printf(" fill_line_buffer- Stash:%s Buffer:%s\n", stash, buffer);
		stash = ft_strjoin(stash, buffer);
		// printf(" fill_line_buffer-4\n");
		// printf(" fill_line_buffer- Stash:%s\n", stash);
	}
	free(buffer);
	// printf("OUT fill_line_buffer\n");
	return (stash);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash;

	// printf("IN get_next_line\n");
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = fill_line_buffer(fd, stash);
	if (!stash)
		return (NULL);
	line = set_line(stash);
	stash = next_line(stash);
	// printf("OUT get_next_line\n");
	return (line);
}
