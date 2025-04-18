#ifndef HOTRACE_H
# define HOTRACE_H

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include "get_next_line.h"

#define GROUP_SIZE 16
#define EMPTY ((int8_t)-128)
// it was 1024 per bucket i increased it by * 10 to hold more data
#define BUCKET_SIZE 10240
#define INIT_GLOBAL_DEPTH 1

typedef struct s_entry
{
	char				*key;
	char				*val;
	u_int64_t			hash;
}						t_entry;

typedef struct s_bucket
{
	int8_t				*ctrl;
	t_entry				*entries;
	size_t				capacity;
	size_t				size;
	size_t				local_depth;
}						t_bucket;

typedef struct s_map
{
	t_bucket			**directory;
	size_t				global_depth;
}	t_map;

#endif
