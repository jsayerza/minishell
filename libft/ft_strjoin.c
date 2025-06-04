/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:16:44 by jsayerza          #+#    #+#             */
/*   Updated: 2025/06/04 19:22:17 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*ptr;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	ptr = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!ptr)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != 0)
		ptr[j++] = s1[i++];
	i = 0;
	while (s2[i] != 0)
		ptr[j++] = s2[i++];
	ptr[j] = 0;
	return (ptr);
}

// char	*ft_strjoin(char *s1, char *s2)
// {
// 	char	*ptr;
// 	int		i;
// 	int		j;

// 	if (!s1)
// 	{
// 		s1 = (char *)malloc(1);
// 		s1[0] = 0;
// 	}
// 	if (!s2)
// 		return (NULL);
// 	ptr = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
// 	if (!ptr)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	while (s1[i] != 0)
// 		ptr[j++] = s1[i++];
// 	i = 0;
// 	while (s2[i] != 0)
// 		ptr[j++] = s2[i++];
// 	ptr[j] = 0;
// 	free(s1);
// 	return (ptr);
// }
