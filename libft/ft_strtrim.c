/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:40:36 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/19 18:02:08 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_find_char_set(char c, char const *set)
{
	while (*set)
		if (*set++ == c)
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	char	*s2;

	i = 0;
	while (s1[i] && (ft_find_char_set(s1[i], set)))
		i++;
	s2 = malloc(sizeof(s1) * ft_strlen(s1) + 1);
	if (!s2)
		return (NULL);
	j = 0;
	while (s1[i])
		s2[j++] = s1[i++];
	j--;
	while (j > 0 && (ft_find_char_set(s2[j], set)))
		j--;
	s2[j + 1] = 0;
	return (s2);
}
