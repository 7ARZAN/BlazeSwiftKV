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

#include "../include/hotrace.h"

uint64_t	djb2(const char *str)
{
	uint64_t	h;
	int			c;

	h = 5381;
	while (*str)
	{
		c = *str;
		h = ((h << 5) + h) + c;
		str++;
	}
	return (h);
}

uint8_t	h2(uint64_t hash)
{
	return ((hash >> 57) & 0x7F);
}
