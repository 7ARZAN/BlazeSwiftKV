#ifndef HOTRACE_H
#define HOTRACE_H

#include <immintrin.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TABLE_SIZE (1 << 20) // 2^20 initial buckets mhm hady hya 1M
#define BUCKET_SIZE 8        // Slots per bucket
#define LOAD_FACTOR 0.85     // Load factor for resizing
#define BUFFER_SIZE (1 << 16) // 64KB input buffer
#define MAX_LINE 1024        // Max line length
#define INITIAL_ARENA_SIZE (1 << 24) // 16MB initial arena

// Metadata byte: 7 bits for H2, 1 bit for empty/occupied
#define EMPTY 0x80
#define OCCUPIED_MASK 0x7F

typedef struct	s_entry
{
    char	*key;
    char	*value;
}		t_entry;

typedef struct	s_bucket
{
    uint8_t	metadata[BUCKET_SIZE]; // H2 hash bits + state
    t_entry	entries[BUCKET_SIZE];  // Key-value pairs
}		t_bucket;

typedef struct s_hotrace
{
    t_bucket	*buckets;    // Array of buckets
    char	*buffer;         // Input buffer
    char	*line;           // Current line
    char	*arena;          // Arena for key/value storage
    size_t	arena_pos;     // Current position in arena
    size_t	arena_size;    // Current arena size
    size_t	buf_pos;       // Buffer position
    size_t	buf_len;       // Buffer length
    size_t	count;         // Number of entries
    size_t	capacity;      // Number of buckets
    int		fd;               // File descriptor
} t_hotrace;

t_hotrace	*init_hotrace(void);
char	*search(t_hotrace *hr, const char *key);
char	*ft_strdup_arena(t_hotrace *hr, const char *s, size_t len);
char	*read_line(t_hotrace *hr);
void	write_str(const char *str);
void	free_hotrace(t_hotrace *hr);
uint64_t	wyhash(const char *str);
uint8_t	h2(uint64_t hash);
int	insert(t_hotrace *hr, const char *key, const char *value);
int	resize_table(t_hotrace *hr);

#endif
