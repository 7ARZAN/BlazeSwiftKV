/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhayyoun <mhayyoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:43:55 by mhayyoun          #+#    #+#             */
/*   Updated: 2025/04/19 21:43:56 by mhayyoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/hotrace.h"

static void	free_hr_content(t_hotrace *hr)
{
	if (!hr)
		return ;
	free(hr->buckets);
	free(hr->buffer);
	free(hr->line);
	free(hr->arena);
}

static t_hotrace	*allocate_hotrace(void)
{
	t_hotrace	*hr;

	hr = malloc(sizeof(t_hotrace));
	if (!hr)
		return (NULL);
	hr->buckets = malloc(TABLE_SIZE * sizeof(t_bucket));
	hr->buffer = malloc(BUFFER_SIZE);
	hr->line = malloc(MAX_LINE);
	hr->arena = malloc(sizeof(t_arena_chunk));
	if (!hr->buckets || !hr->buffer || !hr->line || !hr->arena)
	{
		free_hr_content(hr);
		return (NULL);
	}
	hr->arena->data = malloc(INITIAL_ARENA_SIZE);
	if (!hr->arena->data)
	{
		free_hr_content(hr);
		return (NULL);
	}
	hr->arena->pos = 0;
	hr->arena->size = INITIAL_ARENA_SIZE;
	hr->arena->next = NULL;
	return (hr);
}

t_hotrace	*init_hotrace(void)
{
	t_hotrace	*hr;
	size_t		index;
	size_t		j;

	hr = allocate_hotrace();
	index = 0;
	if (!hr)
		return (NULL);
	while (index < TABLE_SIZE)
	{
		j = 0;
		while (j < BUCKET_SIZE)
		{
			hr->buckets[index].metadata[j] = EMPTY;
			j++;
		}
		index++;
	}
	hr->buf_pos = 0;
	hr->buf_len = 0;
	hr->count = 0;
	hr->capacity = TABLE_SIZE;
	hr->fd = 0;
	return (hr);
}
