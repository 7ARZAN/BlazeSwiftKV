#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 64
# endif

char	*get_next_line(int fd);
int		check_for(char *buff, char c);
char	*ft_strjoin(char *s1, char *s2);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_calloc(size_t count, size_t size);
size_t	ft_strlen(const char *s);

#endif
