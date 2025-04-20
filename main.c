/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhayyoun <mhayyoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:43:51 by mhayyoun          #+#    #+#             */
/*   Updated: 2025/04/20 10:40:43 by mhayyoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"

static inline void	write_str(const char *str)
{
	write(1, str, ft_strlen(str));
}

static int	handle_pair(t_hotrace *hr, char *line)
{
	char	*key;
	char	*value;

	key = ft_strdup_arena(hr, line, ft_strlen(line));
	if (!key)
		return (write_str("Arena Allocation failed for key\n"), 0);
	line = read_line(hr);
	if (!line)
		return (0);
	value = ft_strdup_arena(hr, line, ft_strlen(line));
	if (!value)
		return (write_str("Arena Allocation failed for value\n"), 0);
	if (!insert(hr, key, value))
	{
		write_str("Insertion failed for key: ");
		write_str(key);
		write_str("\n");
		return (0);
	}
	return (1);
}

static void	process_input(t_hotrace *hr)
{
	char	*line;

	while (1)
	{
		line = read_line(hr);
		if (!line || !line[0])
			break ;
		if (!handle_pair(hr, line))
			break ;
	}
}

static void	process_queries(t_hotrace *hr)
{
	char	*line;
	char	*value;

	while (1)
	{
		line = read_line(hr);
		if (!line)
			break ;
		if (!line[0])
			continue ;
		value = search(hr, line);
		if (value)
			write_str(value);
		else
		{
			write_str(line);
			write(1, ": Not found.", 12);
		}
		write(1, "\n", 1);
	}
}

int	main(void)
{
	t_hotrace	*hr;

	hr = init_hotrace();
	if (!hr)
		return (1);
	process_input(hr);
	process_queries(hr);
	free_hotrace(hr);
	return (0);
}
