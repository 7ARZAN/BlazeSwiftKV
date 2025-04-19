#include "hotrace.h"

/*
 * Simplified wyhash for consistency
*/
uint64_t	wyhash(const char *str)
{
	uint64_t		seed;
	const uint8_t	*p;

	seed = 0xa0761d6478bd642fULL;
	p = (const uint8_t *)str;
	while (*p)
	{
		seed ^= (uint64_t) *p++;
		seed *= 0x9e3779b97f4a7c15ULL;
	}
	return (seed ^ (seed >> 32));
}

/*
 * Extract 7 bit h2 for metadata from ur code
*/
uint8_t	h2(uint64_t hash)
{
	return ((hash >> 57) & 0x7F);
}
