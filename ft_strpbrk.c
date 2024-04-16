#include <stdio.h>
#include <string.h>

char *ft_strpbrk(char *str, char *delim)
{
    while (*str)
    {
        if (strchr(delim, *str))
            break;
        str++;
    }
    if (*str)
        return (str);
    return (NULL);
}

void main()
{
	char str[] = "Hello, World!";
	char delim[] = "olW";
	char *res = ft_strpbrk(str, delim);
	if (res != NULL) {
    printf("%c", *res);
} else {
    printf("No matching character found\n");
}
}