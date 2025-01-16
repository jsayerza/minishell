/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 15:59:13 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/19 17:14:03 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*ss;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (NULL);
	ss = malloc(sizeof(s) * len + 1);
	if (!ss)
		return (NULL);
	i = start - 1;
	j = 0;
	while (s[i] && (i < (start - 1 + len)))
		ss[j++] = s[i++];
	ss[j] = 0;
	return (ss);
}
