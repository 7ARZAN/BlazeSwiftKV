#include "hotrace.h"

// Arena-based string duplication with chunk-based allocation
char *ft_strdup_arena(t_hotrace *hr, const char *s, size_t len)
{
    t_arena_chunk *chunk = hr->arena;
    t_arena_chunk *prev = NULL;
    char *dst;

    while (chunk && chunk->pos + len + 1 > chunk->size)
    {
        prev = chunk;
        chunk = chunk->next;
    }
    if (!chunk)
    {
        chunk = malloc(sizeof(t_arena_chunk));
        if (!chunk)
            return NULL;
        chunk->data = malloc(INITIAL_ARENA_SIZE);
        if (!chunk->data)
        {
            free(chunk);
            return NULL;
        }
        chunk->pos = 0;
        chunk->size = INITIAL_ARENA_SIZE;
        chunk->next = NULL;
        if (prev)
            prev->next = chunk;
        else
            hr->arena = chunk;
    }
    dst = chunk->data + chunk->pos;
    __builtin_memcpy(dst, s, len);
    dst[len] = '\0';
    chunk->pos += len + 1;
    return dst;
}

void write_str(const char *str)
{
    write(1, str, __builtin_strlen(str));
}

static char *process_buffer(t_hotrace *hr, size_t *line_pos)
{
    if (hr->buf_pos >= hr->buf_len)
    {
        hr->buf_len = read(hr->fd, hr->buffer, BUFFER_SIZE);
        hr->buf_pos = 0;
        if (hr->buf_len <= 0 && *line_pos)
        {
            hr->line[*line_pos] = '\0';
            return (hr->line);
        }
        if (hr->buf_len <= 0)
            return (NULL);
    }
    return ("");
}

char *read_line(t_hotrace *hr)
{
    size_t line_pos;

    line_pos = 0;
    while (1)
    {
        if (!process_buffer(hr, &line_pos))
            return (NULL);
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
    t_arena_chunk *chunk;
    t_arena_chunk *next;

    if (!hr)
        return;
    free(hr->buckets);
    free(hr->buffer);
    free(hr->line);
    chunk = hr->arena;
    while (chunk)
    {
        next = chunk->next;
        free(chunk->data);
        free(chunk);
        chunk = next;
    }
    free(hr);
}
