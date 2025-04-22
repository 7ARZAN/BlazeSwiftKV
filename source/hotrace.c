/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotrace.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhayyoun <mhayyoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:48:05 by mhayyoun          #+#    #+#             */
/*   Updated: 2025/04/20 17:04:39 by mhayyoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/hotrace.h"

static inline size_t	get_bucket_index(uint64_t hash, size_t capacity)
{
	return (hash & (capacity - 1));
}

static inline int	store_entry(t_store *st, size_t slot, uint8_t h2_val)
{
	t_bucket	*bucket;

	bucket = &st->hr->buckets[st->bucket_idx];
	bucket->metadata[slot] = h2_val;
	bucket->entries[slot].key = ft_strdup_arena(st->hr, st->key, st->k_len);
	bucket->entries[slot].value = ft_strdup_arena(st->hr, st->value, st->v_len);
	if (!bucket->entries[slot].key || !bucket->entries[slot].value)
	{
		bucket->metadata[slot] = EMPTY;
		return (0);
	}
	st->hr->count++;
	return (1);
}

static inline int	insert_recursive_(t_store *st, uint8_t *h2_val,
		size_t *slot, size_t *step)
{
	t_bucket	*bucket;

	while (1)
	{
		bucket = &st->hr->buckets[st->bucket_idx];
		*slot = 0;
		while (*slot < BUCKET_SIZE)
		{
			if (bucket->metadata[*slot] == EMPTY)
				return (store_entry(st, *slot, *h2_val));
			if ((bucket->metadata[*slot] & OCCUPIED_MASK) == *h2_val)
			{
				if (ft_strcmp(bucket->entries[*slot].key, st->key) == 0)
				{
					bucket->entries[*slot].value = ft_strdup_arena(st->hr,
							st->value, st->v_len);
					return (bucket->entries[*slot].value != NULL);
				}
			}
			(*slot)++;
		}
		st->bucket_idx = (st->bucket_idx + *step) & (st->hr->capacity - 1);
		(*step)++;
	}
}

static inline int	insert_recursive(t_store *st, uint64_t hash)
{
	uint8_t	h2_val;
	size_t	step;
	size_t	slot;

	if (st->hr->count >= st->hr->capacity * BUCKET_SIZE * LOAD_FACTOR)
	{
		if (!resize_table(st->hr))
			return (0);
		st->bucket_idx = get_bucket_index(hash, st->hr->capacity);
	}
	step = 1;
	h2_val = h2(hash);
	return (insert_recursive_(st, &h2_val, &slot, &step));
}

int	insert(t_hotrace *hr, t_store *st)
{
	uint64_t	hash;

	hash = djb2(st->key);
	st->hr = hr;
	st->bucket_idx = get_bucket_index(hash, hr->capacity);
	return (insert_recursive(st, hash));
}
