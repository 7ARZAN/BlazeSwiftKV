#include "hotrace.h"

// Process input key-value pairs
static int	handle_pair(t_hotrace *hr, char *line)
{
	char	*key;
	char	*value;

	key = ft_strdup_arena(hr, line, __builtin_strlen(line));
	if (!key)
		return (write_str("Arena Allocation failed for key\n"), 0);
	line = read_line(hr);
	if (!line)
		return (0);
	value = ft_strdup_arena(hr, line, __builtin_strlen(line));
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
			write_str(": Not found.");
		}
		write_str("\n");
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
