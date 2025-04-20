/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hr_search.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhayyoun <mhayyoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:07:27 by mhayyoun          #+#    #+#             */
/*   Updated: 2025/04/20 16:08:01 by mhayyoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"

static inline size_t	get_bucket_index(uint64_t hash, size_t capacity)
{
	return (hash & (capacity - 1));
}

static char inline	*search_recursive(t_hotrace *hr, const char *key,
		uint64_t hash, size_t bucket_idx)
{
	t_bucket	*bucket;
	uint8_t		h2_val;
	size_t		step;
	size_t		slot;

	h2_val = h2(hash);
	step = 1;
	while (1)
	{
		bucket = &hr->buckets[bucket_idx];
		slot = -1;
		while (++slot < BUCKET_SIZE)
		{
			if (bucket->metadata[slot] == EMPTY)
				return (NULL);
			if ((bucket->metadata[slot] & OCCUPIED_MASK) == h2_val)
				if (ft_strcmp(bucket->entries[slot].key, key) == 0)
					return (bucket->entries[slot].value);
		}
		bucket_idx = (bucket_idx + step) & (hr->capacity - 1);
		step++;
	}
}

char	*search(t_hotrace *hr, const char *key)
{
	uint64_t	hash;
	size_t		bucket_idx;

	hash = djb2(key);
	bucket_idx = get_bucket_index(hash, hr->capacity);
	return (search_recursive(hr, key, hash, bucket_idx));
}
