#include "hotrace.h"

static t_hotrace *allocate_hotrace(void)
{
    t_hotrace *hr;

    hr = malloc(sizeof(t_hotrace));
    if (!hr)
	return NULL;
    hr->buckets = malloc(TABLE_SIZE * sizeof(t_bucket));
    hr->buffer = malloc(BUFFER_SIZE);
    hr->line = malloc(MAX_LINE);
    hr->arena = malloc(INITIAL_ARENA_SIZE);
    if (!hr->buckets || !hr->buffer || !hr->line || !hr->arena)
    {
        free(hr->buckets);
        free(hr->buffer);
        free(hr->line);
        free(hr->arena);
        free(hr);
        return NULL;
    }
    return hr;
}

t_hotrace	*init_hotrace(void)
{
    t_hotrace *hr;

    hr = allocate_hotrace();
    if (!hr) return
	NULL;
    for (size_t i = 0; i < TABLE_SIZE; i++)
        for (size_t j = 0; j < BUCKET_SIZE; j++)
            hr->buckets[i].metadata[j] = EMPTY;
    hr->buf_pos = 0;
    hr->buf_len = 0;
    hr->count = 0;
    hr->capacity = TABLE_SIZE;
    hr->fd = 0;
    hr->arena_pos = 0;
    hr->arena_size = INITIAL_ARENA_SIZE;
    return hr;
}
