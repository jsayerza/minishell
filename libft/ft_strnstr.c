/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:22:09 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/16 19:32:04 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (!big)
		return (NULL);
	if (!little)
		return ((char *)big);
	i = 0;
	while (big[i] && i < len)
	{
		j = 0;
		while (big[i + j] && little[j]
			&& big[i + j] == little[j] && i + j < len)
		{
			j++;
		}
		if (!little[j])
			return ((char *)big + i);
		i++;
	}
	return (NULL);
}
/*
int	main(void)
{
	#include <string.h>

	printf("Res:%s\n", ft_strnstr("123456", "45", 4));
	//printf("Ofi:%d\n", strnstr("123456", "234", 3));
	return (0);
}*/