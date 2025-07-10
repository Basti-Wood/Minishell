/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:37:43 by sholz             #+#    #+#             */
/*   Updated: 2024/10/28 13:37:44 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	size_t	total_size;
	void	*ptr;

	total_size = num * size;
	if (num != 0 && total_size / num != size)
	{
		return (NULL);
	}
	ptr = malloc (total_size);
	if (ptr == NULL)
	{
		return (NULL);
	}
	ft_memset(ptr, 0, total_size);
	return (ptr);
}
/*
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Define the number of elements in the array
    int num_elements = 5;

    // Use calloc to allocate memory for the array of integers
    int* array = (int*)ft_calloc(num_elements, sizeof(int));

    // Check if allocation was successful
    if (array == NULL) {
        printf("Memory allocation failed.\n");
        return 1; // Exit the program with an error code
    }

    // Print the initialized values of the array
    printf("Array values after calloc:\n");
    for (int i = 0; i < num_elements; i++) {
        printf("array[%d] = %d\n", i, array[i]); // All values should be 0
    }

    // Assign some values to the array
    for (int i = 0; i < num_elements; i++) {
        array[i] = i + 1; // Assign values 1 to 5
    }

    // Print the updated values of the array
    printf("\nArray values after assignment:\n");
    for (int i = 0; i < num_elements; i++) {
        printf("array[%d] = %d\n", i, array[i]); // Should print values 1 to 5
    }

    // Free the allocated memory
    free(array);
    
    return 0; // Exit the program successfully
}
*/
