#include "hotrace.h"
#include <stdio.h>


/*
 * RobbinHood For collisions !
 * SIMD -- Using Assembly !
 * optimisation insertion and lookup
 * split_bucket function still need some improvment
*/




void	write_str(char *str){
    write(1, str, __builtin_strlen(str));
}

static inline int8_t	h2_(uint64_t hash)
{
	return ((int8_t)(hash & 0x7F));
}

static inline uint64_t	h3_(uint64_t hash, uint64_t depth)
{
	return (hash & ((1 << depth) - 1));
}

uint64_t	hash(const char *key)
{
	uint64_t	h;
	int			c;

	h = 5381;
	while (*key)
	{
		c = *key;
		h = ((h << 5) + h) + c;
		key++;
	}
	return (h);
}

t_bucket	*new_bucket(size_t local_depth)
{
	t_bucket	*new;

	new = malloc(sizeof(t_bucket));
	if (!new)
		return (NULL);
	new->capacity = BUCKET_SIZE;
	new->size = 0;
	new->local_depth = local_depth;
	new->ctrl = malloc(BUCKET_SIZE + GROUP_SIZE);
	if (!new->ctrl)
		return (free(new), NULL);
	memset(new->ctrl, EMPTY, BUCKET_SIZE + GROUP_SIZE);
	new->entries = calloc(BUCKET_SIZE, sizeof(t_entry));
	if (!new->entries)
		return (free(new->ctrl), free(new), NULL);
	return (new);
}

t_map	*new_hash_map(void)
{
	t_map	*new;
	int		i;

	new = malloc(sizeof(t_map));
	if (!new)
		return (NULL);
	new->global_depth = INIT_GLOBAL_DEPTH;
	new->directory = malloc(sizeof(t_bucket *) * (1 << new->global_depth));
	i = 0;
	while (i < (1 << new->global_depth))
	{
		new->directory[i] = new_bucket(new->global_depth);
		i++;
	}
	return (new);
}

void	dup_directory(t_map *map)
{
	size_t	old_size;
	size_t	i;
	size_t	new_size;

	i = 0;
	old_size = 1 << (map->global_depth);
	new_size = 1 << (map->global_depth + 1);
	map->directory = realloc(map->directory, sizeof(t_bucket *) * new_size);
	while (i < old_size)
	{
		map->directory[i + old_size] = map->directory[i];
		i++;
	}
	map->global_depth++;
}

void	split_bucket(t_map *map, u_int64_t idx)
{
	t_bucket	*old;
	u_int64_t	new_depth;
	t_bucket	*new;
	t_entry		tmp[BUCKET_SIZE + 1];
	size_t		i;
	size_t		old_count;
	int8_t		h2;
	u_int64_t	h3;
	t_bucket	*target;
	size_t		pos;

	old = map->directory[idx];
	new_depth = ++old->local_depth;
	if (new_depth > map->global_depth)
		dup_directory(map);
	new = new_bucket(new_depth);
	i = 0;
	while (i < (1 << map->global_depth))
	{
		if (map->directory[i] == old && (i & (1 << (new_depth - 1))))
			map->directory[i] = new;
		i++;
	}
	memcpy(tmp, old->entries, sizeof(t_entry) * old->size);
	old_count = old->size;
	memset(old->ctrl, EMPTY, old->capacity);
	memset(old->entries, 0, sizeof(t_entry) * old->capacity);
	old->size = 0;
	i = 0;
	while (i < old_count)
	{
		h2 = h2_(tmp[i].hash);
		h3 = h3_(tmp[i].hash, map->global_depth);
		target = map->directory[h3];
		idx = tmp[i].hash % target->capacity;
		for (int j = 0; j < target->capacity; j++)
		{
			pos = (idx + j) % target->capacity;
			if (target->ctrl[pos] == EMPTY)
			{
				target->ctrl[pos] = h2;
				target->entries[pos].key = tmp[i].key;
				target->entries[pos].val = tmp[i].val;
				target->size++;
				break ;
			}
		}
		i++;
	}
}

void	hash_map_insert(t_map *map, char *key, char *value)
{
	u_int64_t	h1;
	int8_t		h2;
	u_int64_t	h3;
	size_t		idx;
	int			i;
	size_t		pos;
	t_bucket	*b;

	h1 = hash(key);
	h2 = h2_(h1);
	h3 = h3_(h1, map->global_depth);
	b = map->directory[h3];
	if (b->size < b->capacity)
	{
		idx = h1 % b->capacity;
		for (i = 0; i < b->capacity; i++)
		{
			pos = (idx + i) % b->capacity;
			if (b->ctrl[pos] == EMPTY)
			{
				b->ctrl[pos] = h2;
				b->entries[pos].key = key;
				b->entries[pos].val = value;
				b->entries[pos].hash = h1;
				b->size++;
				// write_str("Inserting key=%s, value=value%s\n", key, value);
				return ;
			}
		}
	}
	else
	{
		split_bucket(map, h3);
		hash_map_insert(map, key, value);
	}
}

char	*hash_map_get(t_map *map, char *key)
{
	u_int64_t	h1;
	int8_t		h2;
	u_int64_t	h3;
	size_t		idx;
	int			i;
	size_t		pos;
	t_bucket	*b;

	h1 = hash(key);
	h2 = h2_(h1);
	h3 = h3_(h1, map->global_depth);
	b = map->directory[h3];
	idx = h1 % b->capacity;
	while (1)
	{
		i = 0;
		while (i < b->capacity)
		{
			pos = (idx + i) % b->capacity;
			if (b->ctrl[pos] == EMPTY)
				return (NULL);
			if (b->ctrl[pos] == h2 && strcmp(b->entries[pos].key, key) == 0)
				return (b->entries[pos].val);
			i++;
		}
		idx = (idx + GROUP_SIZE) % b->capacity;
	}
}

void	read_input(t_map *map)
{
	char			*line;
	t_entry			dict;
	unsigned long	n;
	int				fd;

	n = 0;
	dict = (t_entry){.key = 0, .val = 0};
	fd = open("hotrace.txt", O_RDONLY);
	while (1)
	{
		line = get_next_line(0);
		if (!line || !*line)
			break ;
		if (!dict.key)
			dict.key = line;
		else
			dict.val = line;
		if (dict.val)
		{
			hash_map_insert(map, dict.key, dict.val);
			dict = (t_entry){.key = 0, .val = 0};
			n++;
		// 	if (n % 100000 == 0 && n != 0)
		// 	{
		// 		write_str("%lu\n", n);
		// 	}
		}
	}
	close(fd);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000L + tv.tv_usec);
}

void	search(t_map *map, unsigned long *avg, unsigned long *count)
{
	char	*line;
	char	*res;
	long	start;
	long	end;

	while (1)
	{
		line = get_next_line(0);
		if (!line)
			break ;
		if (*line)
		{
		///write_str("line=%s\n", line);
			start = get_time();
			res = hash_map_get(map, line);
			end = get_time() - start;
			if (!res){
			    write_str(line);
			    write_str(": Not found");
			}else
			    write_str(res);
			write(1, "\n", 1);
		///	write_str("(%s), took=%ld\n", res, end);
			if (end >= 0)
				*avg += end;
			*count += 1;
		}
		else
		{
			break ;
		}
		free(line);
	}
}

int	main(void)
{
	t_map			*map;
	unsigned long	avg;
	unsigned long	count;
	long			start;
	long			end;

	// t_bucket		*map;
	avg = 0;
	count = 0;
	map = new_hash_map();
	if (!map)
		return (1);
	start = get_time();
	read_input(map);
	write_str("Searching....\n");
	search(map, &avg, &count);
	end = get_time() - start;
	printf("took=%ld ms\n", end);
	printf("totalAVG=%lu ms, totalCount=%lu, AVG=%lf ms\n", avg, count,
		(double)avg / (double)count);
	// hash_map_insert(map, "Hello", "world!");
	// hash_map_insert(map, "Name", "Hayyoun");
	// write_str("%s\n", hash_map_get(map, "Hello"));
	return (0);
}
