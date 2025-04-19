#ifndef HOTRACE_H
#define HOTRACE_H

#include <immintrin.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TABLE_SIZE 1048576 // 2^20 initial buckets
#define BUCKET_SIZE 8        // Slots per bucket
#define LOAD_FACTOR 0.75     // Load factor for resizing
#define BUFFER_SIZE 65536 // 64KB input buffer
#define MAX_LINE 10240        // Max line length
#define INITIAL_ARENA_SIZE 1073741824

// Metadata byte: 7 bits for H2, 1 bit for empty/occupied
#define EMPTY 0x80
#define OCCUPIED_MASK 0x7F

typedef struct s_entry
{
    char *key;
    char *value;
} t_entry;

typedef struct	s_bucket
{
    uint8_t metadata[BUCKET_SIZE];		// H2 hash bits + state
    t_entry entries[BUCKET_SIZE];		// Key-value pairs
}		t_bucket;

typedef struct	s_arena_chunk
{
    char			*data;          // Chunk memory
    size_t			pos;            // Current position in chunk
    size_t			size;           // Chunk size (fixed)
    struct s_arena_chunk	*next;		// Next chunk
}		t_arena_chunk;

typedef struct s_hotrace
{
    t_arena_chunk	*arena;     		// Linked list of arena chunks
    t_bucket		*buckets;        	// Array of buckets
    char		*buffer;             	// Input buffer
    char		*line;             	// Current line
    size_t		buf_pos;           	// Buffer position
    size_t		buf_len;           	// Buffer length
    size_t		count;             	// Number of entries
    size_t		capacity;          	// Number of buckets
    int			fd;                   	// File descriptor
} t_hotrace;

t_hotrace *init_hotrace(void);
char *search(t_hotrace *hr, const char *key);
char *ft_strdup_arena(t_hotrace *hr, const char *s, size_t len);
char *read_line(t_hotrace *hr);
void write_str(const char *str);
void free_hotrace(t_hotrace *hr);
uint64_t wyhash(const char *str);
uint8_t h2(uint64_t hash);
int insert(t_hotrace *hr, const char *key, const char *value);
int resize_table(t_hotrace *hr);

#endif
