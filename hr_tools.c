#include "hotrace.h"

// Arena-based string duplication with dynamic resizing
char	*ft_strdup_arena(t_hotrace *hr, const char *s, size_t len)
{
    char	*new_arena;
    char	*dst;
    size_t	new_size;

    if (hr->arena_pos + len + 1 > hr->arena_size)
    {
        new_size = hr->arena_size * 2;
        while (new_size < hr->arena_pos + len + 1) new_size *= 2;
        new_arena = __builtin_realloc(hr->arena, new_size);
        if (!new_arena)
	    return NULL;
        hr->arena = new_arena;
        hr->arena_size = new_size;
    }
    dst = hr->arena + hr->arena_pos;
    __builtin_memcpy(dst, s, len);
    dst[len] = '\0';
    hr->arena_pos += len + 1;
    return dst;
}

void	write_str(const char *str)
{
    write(1, str, __builtin_strlen(str));
}

static char	*process_buffer(t_hotrace *hr, size_t *line_pos)
{
    if (hr->buf_pos >= hr->buf_len)
    {
        hr->buf_len = read(hr->fd, hr->buffer, BUFFER_SIZE);
        hr->buf_pos = 0;
        if (hr->buf_len <= 0 && *line_pos)
	{
            hr->line[*line_pos] = '\0';
            return hr->line;
        }
        if (hr->buf_len <= 0)
	    return NULL;
    }
    return "";
}

char *read_line(t_hotrace *hr)
{
    size_t line_pos;

    line_pos = 0;
    while (1)
    {
        if (!process_buffer(hr, &line_pos))
	    return NULL;
        if (hr->buf_pos < hr->buf_len)
	{
            hr->line[line_pos] = hr->buffer[hr->buf_pos++];
            if (hr->line[line_pos] == '\n')
	    {
                hr->line[line_pos] = '\0';
                return hr->line;
            }
            if (line_pos < MAX_LINE - 1)
		line_pos++;
        }
    }
}

void free_hotrace(t_hotrace *hr)
{
    if (!hr)
	return;
    free(hr->buckets);
    free(hr->buffer);
    free(hr->line);
    free(hr->arena);
    free(hr);
}
