/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 10:29:25 by mmasarov          #+#    #+#             */
/*   Updated: 2024/05/23 15:00:51 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <printf.h>
# include <stdarg.h>
# include <limits.h>
# include <fcntl.h>

/* part one functions*/
int		ft_atoi(const char *nptr);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_isprint(int c);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strdup(const char *s);
size_t	ft_strlcat(char *dst, const char *src, size_t dest_size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_tolower(int c);
int		ft_toupper(int c);

/* part two functions*/
//char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);

/* Added functions*/
char	*ft_strtok(char *str, const char *delim);
char	*ft_strpbrk(char *str, char *delim);
char	*ft_strncat(char *dest, const char *src, size_t n);
char	*ft_strcat(char *dest, const char *src);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strncpy(char *dest, const char *src, size_t n);

/* Printf added*/
//int	ft_printf(const char *print, ...);
int		ft_printf_fd(unsigned int fd, const char *print, ...);
int		ft_vprintf_fd(unsigned int fd, const char *print, va_list args);

/* get_next_line added*/
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

//# include <stdio.h>
//# include <stdlib.h>
//# include <unistd.h>

size_t	ft_strlen_gnl(char *str);
void	my_free_gnl(char **str);
char	*ft_strjoin_gnl(char *read_str, char *buff);
char	*del_buf_return_res(char **buffer, char *res, int len_to_del);
char	*extract_til_nl_or_end(char **buff);
char	*read_buff_size(int fd, char **buffer);
int		check_nl(char *str);
char	*get_next_line(int fd);

#endif
