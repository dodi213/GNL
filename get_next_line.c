/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharoud <mharoud42@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 21:06:43 by mharoud           #+#    #+#             */
/*   Updated: 2026/10/19 21:06:52 by mharoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_to_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	bytes;

	bytes = 1;
	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!buf)
		bytes = -1;
	while (bytes > 0 && !gnl_strchr(stash, '\n'))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes >= 0)
		{
			buf[bytes] = '\0';
			stash = gnl_strjoin(stash, buf);
			if (!stash)
				bytes = -1;
		}
	}
	free(buf);
	if (bytes < 0)
	{
		free(stash);
		return (NULL);
	}
	return (stash);
}

static char	*split_stash(char **stash)
{
	char	*line;
	char	*rest;
	size_t	len;

	len = 0;
	while ((*stash)[len] && (*stash)[len] != '\n')
		len++;
	if ((*stash)[len] == '\n')
		len++;
	line = gnl_substr(*stash, 0, len);
	rest = NULL;
	if (line && (*stash)[len])
	{
		rest = gnl_substr(*stash, len, gnl_strlen(*stash) - len);
		if (!rest)
		{
			free(line);
			line = NULL;
		}
	}
	free(*stash);
	*stash = rest;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stash;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_to_stash(fd, stash);
	if (!stash)
		return (NULL);
	if (!*stash)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	return (split_stash(&stash));
}
