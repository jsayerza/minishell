/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 18:53:01 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/18 19:11:40 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ss;

	ss = (unsigned char *)s;
	while (*ss && n--)
	{
		if (*ss == (char)c)
			return ((void *)ss);
		ss++;
	}
	return (0);
}
