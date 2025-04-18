#include "hotrace.h"

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
	printf("Searching....\n");
	search(map, &avg, &count);
	end = get_time() - start;
	printf("took=%ld ms\n", end);
	printf("totalAVG=%lu ms, totalCount=%lu, AVG=%lf ms\n", avg, count,
		(double)avg / (double)count);
	// hash_map_insert(map, "Hello", "world!");
	// hash_map_insert(map, "Name", "Hayyoun");
	// printf("%s\n", hash_map_get(map, "Hello"));
	return (0);
}
