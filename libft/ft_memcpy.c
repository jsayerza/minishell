/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:07:29 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/18 16:34:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	if ((dest == NULL) && (src == NULL))
		return (NULL);
	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	while (*s && n-- > 0)
		*d++ = *s++;
	return (dest);
}
