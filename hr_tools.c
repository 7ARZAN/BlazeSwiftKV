#include "hotrace.h"

// char	*read_line(t_hotrace *hr)
// {
// 	size_t	line_pos;
//
// 	line_pos = 0;
// 	while (1)
// 	{
// 		if (!process_buffer(hr, &line_pos))
// 			return (NULL);
// 		if (hr->buf_pos < hr->buf_len)
// 		{
// 			hr->line[line_pos] = hr->buffer[hr->buf_pos++];
// 			if (hr->line[line_pos] == '\n')
// 			{
// 				hr->line[line_pos] = '\0';
// 				return (hr->line);
// 			}
// 			if (line_pos < MAX_LINE - 1)
// 				line_pos++;
// 		}
// 	}
// }
