/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:40:36 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/19 18:02:08 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	words_count(char const *s, char c)
{
	int		word;
	int		wcount;

	word = 0;
	wcount = 0;
	while (*s != '\0')
	{
		if (*s != c && !word)
		{
			wcount++;
			word = 1;
		} else if (*s == c && word)
			word = 0;
		s++;
	}
	return (wcount);
}

int	word_len(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

void	free_mem(char **array, int i)
{
	i--;
	while (i >= 0)
		free(array[i--]);
	free(array);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		wordlen;
	int		i;

	array = (char **)malloc(sizeof(char *) * words_count(s, c) + 1);
	if (!array)
		return (NULL);
	i = 0;
	while (*s != 0)
	{
		if (*s != c)
		{
			wordlen = word_len((char *)s, c);
			array[i] = malloc(sizeof(s) * wordlen + 1);
			if (!array[i])
			{
				free_mem(array, i);
				return (NULL);
			}
			ft_strlcpy(array[i++], (char *)s, wordlen + 1);
			s = s + wordlen;
		} else
		s++;
	}
	array[i] = 0;
	return (array);
}