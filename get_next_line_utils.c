/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhayyoun <mhayyoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:51:36 by mhayyoun          #+#    #+#             */
/*   Updated: 2025/01/30 14:16:52 by mhayyoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*new;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	new = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!new)
		return (NULL);
	ft_memcpy(new, s1, len1);
	ft_memcpy(new + len1, s2, len2);
	new[len1 + len2] = '\0';
	return (new);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	if (dst == src || n == 0)
		return (dst);
	i = 0;
	while (i < n)
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dst);
}

void	*ft_calloc(size_t count, size_t size)
{
	void			*aloc;
	unsigned char	*p;

	size *= count;
	aloc = malloc(size);
	if (aloc)
	{
		p = aloc;
		while (size--)
			*p++ = 0;
	}
	return (aloc);
}

int	check_for(char *buff, char c)
{
	int	i;

	i = 0;
	while (buff && buff[i])
	{
		if (buff[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s && ((const unsigned char *)s)[len] != '\0')
		len++;
	return (len);
}
