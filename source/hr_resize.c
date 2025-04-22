/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hr_resize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhayyoun <mhayyoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:33:56 by mhayyoun          #+#    #+#             */
/*   Updated: 2025/04/20 16:39:00 by mhayyoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/hotrace.h"

static inline void	resize_helper_(t_hotrace *hr, t_bucket *old_buckets,
		size_t old_capacity)
{
	size_t	i;
	size_t	j;
	t_store	st;

	hr->count = 0;
	i = 0;
	while (i < old_capacity)
	{
		j = 0;
		while (j < BUCKET_SIZE)
		{
			if (old_buckets[i].metadata[j] != EMPTY)
			{
				st = (t_store){.key = old_buckets[i].entries[j].key,
					old_buckets[i].entries[j].value};
				st.k_len = ft_strlen(st.key);
				st.v_len = ft_strlen(st.value);
				insert(hr, &st);
			}
			j++;
		}
		i++;
	}
}

static inline void	resize_helper(t_hotrace *hr, t_bucket *old_buckets,
		size_t old_capacity)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < hr->capacity)
	{
		j = 0;
		while (j < BUCKET_SIZE)
		{
			hr->buckets[i].metadata[j] = EMPTY;
			j++;
		}
		i++;
	}
	resize_helper_(hr, old_buckets, old_capacity);
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
	resize_helper(hr, old_buckets, old_capacity);
	free(old_buckets);
	return (1);
}
