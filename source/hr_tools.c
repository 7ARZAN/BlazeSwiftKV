/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hr_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhayyoun <mhayyoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:44:01 by mhayyoun          #+#    #+#             */
/*   Updated: 2025/04/20 16:21:31 by mhayyoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/hotrace.h"

static inline int	extend_arena(t_hotrace *hr, t_arena_chunk **chunk,
		t_arena_chunk *prev)
{
	(*chunk) = malloc(sizeof(t_arena_chunk));
	if (!(*chunk))
		return (0);
	(*chunk)->data = malloc(INITIAL_ARENA_SIZE);
	if (!(*chunk)->data)
	{
		free((*chunk));
		return (0);
	}
	(*chunk)->pos = 0;
	(*chunk)->size = INITIAL_ARENA_SIZE;
	(*chunk)->next = NULL;
	if (prev)
		prev->next = *chunk;
	else
		hr->arena = *chunk;
	return (1);
}

char	*ft_strdup_arena(t_hotrace *hr, const char *s, size_t len)
{
	t_arena_chunk	*chunk;
	t_arena_chunk	*prev;
	char			*dst;

	chunk = hr->arena;
	prev = NULL;
	while (chunk && chunk->pos + len + 1 > chunk->size)
	{
		prev = chunk;
		chunk = chunk->next;
	}
	if (!chunk && !extend_arena(hr, &chunk, prev))
		return (NULL);
	dst = chunk->data + chunk->pos;
	ft_memcpy(dst, s, len);
	dst[len] = '\0';
	chunk->pos += len + 1;
	return (dst);
}

static char	*process_buffer(t_hotrace *hr, size_t *line_pos)
{
	if (hr->buf_pos >= hr->buf_len)
	{
		hr->buf_len = read(hr->fd, hr->buffer, BUFFER_SIZE);
		hr->buf_pos = 0;
		if (hr->buf_len <= 0 && *line_pos)
		{
			hr->line[*line_pos] = '\0';
			return (hr->line);
		}
		if (hr->buf_len <= 0)
			return (NULL);
	}
	return ("");
}

char	*read_line(t_hotrace *hr, size_t *len)
{
	(*len) = 0;
	while (1)
	{
		if (!process_buffer(hr, &(*len)))
			return (NULL);
		if (hr->buf_pos < hr->buf_len)
		{
			hr->line[(*len)] = hr->buffer[hr->buf_pos++];
			if (hr->line[(*len)] == '\n')
			{
				hr->line[(*len)] = '\0';
				return (hr->line);
			}
			if ((*len) < MAX_LINE - 1)
				(*len)++;
		}
	}
}

void	free_hotrace(t_hotrace *hr)
{
	t_arena_chunk	*chunk;
	t_arena_chunk	*next;

	if (!hr)
		return ;
	free(hr->buckets);
	free(hr->buffer);
	free(hr->line);
	chunk = hr->arena;
	while (chunk)
	{
		next = chunk->next;
		free(chunk->data);
		free(chunk);
		chunk = next;
	}
	free(hr);
}
