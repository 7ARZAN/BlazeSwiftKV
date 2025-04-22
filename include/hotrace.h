/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotrace.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhayyoun <mhayyoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:44:09 by mhayyoun          #+#    #+#             */
/*   Updated: 2025/04/20 16:45:03 by mhayyoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOTRACE_H
# define HOTRACE_H

# include <stdlib.h>
# include <unistd.h>

# define TABLE_SIZE 1048576
# define BUCKET_SIZE 32
# define LOAD_FACTOR 0.75
# define BUFFER_SIZE 65536
# define MAX_LINE 10240
# define INITIAL_ARENA_SIZE 1073741824

# define EMPTY 0x80
# define OCCUPIED_MASK 0x7F

typedef struct s_entry
{
	char					*key;
	char					*value;
}							t_entry;

typedef struct s_bucket
{
	uint8_t					metadata[BUCKET_SIZE];
	t_entry					entries[BUCKET_SIZE];
}							t_bucket;

typedef struct s_arena_chunk
{
	char					*data;
	size_t					pos;
	size_t					size;
	struct s_arena_chunk	*next;
}							t_arena_chunk;

typedef struct s_hotrace
{
	t_arena_chunk			*arena;
	t_bucket				*buckets;
	char					*buffer;
	char					*line;
	size_t					buf_pos;
	size_t					buf_len;
	size_t					count;
	size_t					capacity;
	int						fd;
}							t_hotrace;

typedef struct s_store
{
	t_hotrace				*hr;
	const char				*key;
	const char				*value;
	size_t					v_len;
	size_t					k_len;
	size_t					bucket_idx;
}							t_store;

t_hotrace					*init_hotrace(void);
char						*search(t_hotrace *hr, const char *key);
char						*ft_strdup_arena(t_hotrace *hr, const char *s,
								size_t len);
char						*read_line(t_hotrace *hr, size_t *len);
void						free_hotrace(t_hotrace *hr);
uint64_t					djb2(const char *str);
uint8_t						h2(uint64_t hash);
int							insert(t_hotrace *hr, t_store *st);
int							resize_table(t_hotrace *hr);
size_t						ft_strlen(const char *s);
void						*ft_memcpy(void *dst, const void *src, size_t n);
int							ft_strcmp(const char *s1, const char *s2);

#endif
