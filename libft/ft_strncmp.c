/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:22:09 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/16 19:32:04 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 1;
	while (*s1 && *s2 && *s1 == *s2 && i <= n)
	{
		s1++;
		s2++;
		i++;
	}
	if (i > n)
		return (0);
	return (*s1 - *s2);
}
/*
int	main(void)
{
	#include <string.h>

	printf("Res:%d\n", ft_strncmp("1234536", "123536", 3));
	printf("Ofi:%d\n", strncmp("1234536", "123536", 3));
	return (0);
}*/
