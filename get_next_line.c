/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhayyoun <mhayyoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:51:06 by mhayyoun          #+#    #+#             */
/*   Updated: 2025/04/17 18:28:36 by mhayyoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

long	ft_helper(long r, char **buff, char **new)
{
	if (r <= 0)
	{
		free(*new);
		*new = NULL;
		if (r == -1)
		{
			free(*buff);
			*buff = NULL;
		}
		return (0);
	}
	return (1);
}

char	*read_file(int fd, char **buff)
{
	char	*new;
	char	*tmp;
	long	r;

	new = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!new)
		return (free(*buff), *buff = NULL, NULL);
	while (check_for(*buff, '\n') == -1)
	{
		r = read(fd, new, BUFFER_SIZE);
		if (!ft_helper(r, buff, &new))
			return (NULL);
		new[r] = 0;
		tmp = ft_strjoin(*buff, new);
		free(*buff);
		if (!tmp)
			return (*buff = NULL, NULL);
		*buff = tmp;
	}
	return (free(new), *buff);
}

char	*ft_get_next(char **buff)
{
	char	*new;
	int		i;
	size_t	buff_len;
	size_t	new_len;

	i = check_for(*buff, '\n');
	if (i == -1)
		return (free(*buff), *buff = NULL, NULL);
	i++;
	buff_len = ft_strlen(*buff);
	new_len = buff_len - i;
	new = ft_calloc(new_len + 1, sizeof(char));
	if (!new)
		return (free(*buff), *buff = NULL, NULL);
	ft_memcpy(new, *buff + i, new_len);
	return (free(*buff), *buff = NULL, new);
}

char	*ft_line(char *buff)
{
	char	*new;
	int		i;

	i = 0;
	if (!buff[i])
		return (NULL);
	while (buff[i] && buff[i] != '\n')
		i++;
	// if (buff[i] == '\n')
	// 	i++;
	new = ft_calloc(i + 1, sizeof(char));
	if (!new)
		return (NULL);
	ft_memcpy(new, buff, i);
	return (new);
}

char	*get_next_line(int fd)
{
	static char	*buff[4096];
	char		*new;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(buff[fd]);
		buff[fd] = NULL;
		return (NULL);
	}
	read_file(fd, &buff[fd]);
	if (!buff[fd])
		return (NULL);
	new = ft_line(buff[fd]);
	buff[fd] = ft_get_next(&buff[fd]);
	return (new);
}
