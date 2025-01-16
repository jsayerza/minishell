/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 17:40:36 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/19 18:02:08 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*cpystr(char str[11], int is_neg, int i)
{
	int		j;
	char	*res;

	res = malloc(sizeof(char) * (11 - i + 1 + is_neg));
	if (!res)
		return (NULL);
	j = 0;
	if (is_neg)
		res[j++] = '-';
	while (i <= 10)
		res[j++] = str[i++];
	res[j] = '\0';
	return (res);
}

char	*ft_itoa(int n)
{
	char			str[11];
	int				is_neg;
	int				i;
	unsigned int	nn;

	is_neg = (n < 0);
	nn = n;
	if (is_neg)
		nn = -nn;
	i = 10;
	while (nn / 10 > 0)
	{
		str[i--] = nn % 10 + '0';
		nn = nn / 10;
	}
	str[i] = nn % 10 + '0';
	return (cpystr(str, is_neg, i));
}