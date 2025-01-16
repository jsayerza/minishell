/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:47:14 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/19 13:36:48 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	aton(int i, int j, char *array)
{
	unsigned int	n;

	n = 0;
	i = 0;
	while (i < j)
		n = n * 10 + (array[i++] - '0');
	return (n);
}

int	ft_atoi(const char *nptr)
{
	int				sign;
	int				i;
	int				j;
	char			array[11];

	sign = 1;
	i = 0;
	j = 0;
	if (nptr[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (nptr[i] && (nptr[i] >= '0' && nptr[i] <= '9'))
		array[j++] = nptr[i++];
	return (sign * aton(i, j, array));
}
