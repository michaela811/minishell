/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:08 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 16:29:47 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void *new_ptr;
	size_t copy_size;

    // If new_size is zero, free the memory and return NULL
    if (new_size == 0)
		return (free(ptr) , NULL);
    // If ptr is NULL, allocate new memory
    if (ptr == NULL)
        return (malloc(new_size));
    // Allocate new memory
    new_ptr = malloc(new_size);
    if (!new_ptr)
        return (NULL);
    // Copy the old data to the new memory block
    if (old_size < new_size)
        copy_size = old_size;
    else
        copy_size = new_size;
    ft_memcpy(new_ptr, ptr, copy_size);
    // Free the old memory block
    free(ptr);
    // Return the new memory block
    return (new_ptr);
}