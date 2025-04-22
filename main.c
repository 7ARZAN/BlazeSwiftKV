/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhayyoun <mhayyoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:43:51 by mhayyoun          #+#    #+#             */
/*   Updated: 2025/04/20 16:36:20 by mhayyoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/hotrace.h"

static inline void	write_str(const char *str)
{
	write(1, str, ft_strlen(str));
}

static int	handle_pair(t_hotrace *hr, char *line, size_t *len)
{
	char	*key;
	char	*value;
	t_store	st;

	key = ft_strdup_arena(hr, line, *len);
	if (!key)
		return (write_str("Arena Allocation failed for key\n"), 0);
	st.k_len = *len;
	line = read_line(hr, len);
	if (!line)
		return (0);
	value = ft_strdup_arena(hr, line, *len);
	if (!value)
		return (write_str("Arena Allocation failed for value\n"), 0);
	st = (t_store){.key = key, .value = value, .k_len = st.k_len,
		.v_len = *len};
	if (!insert(hr, &st))
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
	size_t	len;

	while (1)
	{
		line = read_line(hr, &len);
		if (!line || !line[0])
			break ;
		if (!handle_pair(hr, line, &len))
			break ;
	}
}

static void	process_queries(t_hotrace *hr)
{
	char	*line;
	char	*value;
	size_t	len;

	while (1)
	{
		line = read_line(hr, &len);
		if (!line)
			break ;
		if (!line[0])
			continue ;
		value = search(hr, line);
		if (value)
		{
			write_str(value);
			write(1, "\n", 1);
		}
		else
		{
			write(1, line, len);
			write(1, ": Not found.\n", 13);
		}
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
