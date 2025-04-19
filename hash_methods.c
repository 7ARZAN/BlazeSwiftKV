/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_methods.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhayyoun <mhayyoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:44:21 by mhayyoun          #+#    #+#             */
/*   Updated: 2025/04/19 21:44:21 by mhayyoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"

uint64_t	wyhash(const char *str)
{
	uint64_t		seed;
	const uint8_t	*p;

	seed = 0xa0761d6478bd642fULL;
	p = (const uint8_t *)str;
	while (*p)
	{
		seed ^= (uint64_t) * p++;
		seed *= 0x9e3779b97f4a7c15ULL;
	}
	return (seed ^ (seed >> 32));
}

uint8_t	h2(uint64_t hash)
{
	return ((hash >> 57) & 0x7F);
}
