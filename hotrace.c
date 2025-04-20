/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotrace.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhayyoun <mhayyoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:48:05 by mhayyoun          #+#    #+#             */
/*   Updated: 2025/04/20 10:17:53 by mhayyoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"

static inline size_t	get_bucket_index(uint64_t hash, size_t capacity)
{
	return (hash & (capacity - 1));
}

static int	store_entry(t_hotrace *hr, size_t bucket_idx, size_t slot,
		const char *key, const char *value, uint8_t h2_val)
{
	t_bucket	*bucket;

	bucket = &hr->buckets[bucket_idx];
	bucket->metadata[slot] = h2_val;
	bucket->entries[slot].key = ft_strdup_arena(hr, key, ft_strlen(key));
	bucket->entries[slot].value = ft_strdup_arena(hr, value, ft_strlen(value));
	if (!bucket->entries[slot].key || !bucket->entries[slot].value)
	{
		bucket->metadata[slot] = EMPTY;
		return (0);
	}
	hr->count++;
	return (1);
}

static int	insert_recursive(t_store *st, uint64_t hash)
{
	t_bucket	*bucket;
	uint8_t		h2_val;
	size_t		step;
	size_t		slot;

	if (st->hr->count >= st->hr->capacity * BUCKET_SIZE * LOAD_FACTOR)
	{
		if (!resize_table(st->hr))
			return (0);
		st->bucket_idx = get_bucket_index(hash, st->hr->capacity);
	}
	h2_val = h2(hash);
	step = 1;
	while (1)
	{
		bucket = &st->hr->buckets[st->bucket_idx];
		slot = 0;
		while (slot < BUCKET_SIZE)
		{
			if (bucket->metadata[slot] == EMPTY)
				return (store_entry(st->hr, st->bucket_idx, slot, st->key,
						st->value, h2_val));
			if ((bucket->metadata[slot] & OCCUPIED_MASK) == h2_val)
			{
				if (ft_strcmp(bucket->entries[slot].key, st->key) == 0)
				{
					bucket->entries[slot].value = ft_strdup_arena(st->hr,
							st->value, ft_strlen(st->value));
					return (bucket->entries[slot].value != NULL);
				}
			}
			slot++;
		}
		st->bucket_idx = (st->bucket_idx + step) & (st->hr->capacity - 1);
		step++; // Quadratic probing
	}
}

int	insert(t_hotrace *hr, const char *key, const char *value)
{
	uint64_t	hash;
	size_t		bucket_idx;
	t_store		store;

	hash = djb2(key);
	bucket_idx = get_bucket_index(hash, hr->capacity);
	store = (t_store){.hr = hr, .key = key, .value = value};
	store.bucket_idx = get_bucket_index(hash, hr->capacity);
	return (insert_recursive(&store, hash));
}

static char	*search_recursive(t_hotrace *hr, const char *key, uint64_t hash,
		size_t bucket_idx)
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

int	resize_table(t_hotrace *hr)
{
	t_bucket	*old_buckets;
	size_t		old_capacity;

	old_buckets = hr->buckets;
	old_capacity = hr->capacity;
	hr->capacity *= 2;
	hr->buckets = malloc(hr->capacity * sizeof(t_bucket));
	if (!hr->buckets)
	{
		hr->buckets = old_buckets;
		hr->capacity = old_capacity;
		return (0);
	}
	for (size_t i = 0; i < hr->capacity; i++)
		for (size_t j = 0; j < BUCKET_SIZE; j++)
			hr->buckets[i].metadata[j] = EMPTY;
	hr->count = 0;
	for (size_t i = 0; i < old_capacity; i++)
		for (size_t j = 0; j < BUCKET_SIZE; j++)
			if (old_buckets[i].metadata[j] != EMPTY)
				insert(hr, old_buckets[i].entries[j].key,
					old_buckets[i].entries[j].value);
	free(old_buckets);
	return (1);
}
