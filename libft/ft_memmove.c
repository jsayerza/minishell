/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:37:37 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/18 18:41:03 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*d;
	char	*s;
	size_t	i;

	if (!dest && !src)
		return (NULL);
	d = (char *)dest;
	s = (char *)src;
	i = 0;
	if (d <= s)
	{
		while (n--)
			*d++ = *s++;
	}
	else if (d > s)
	{
		while (++i <= n)
			d[n - i] = s[n - i];
	}
	return (dest);
}
