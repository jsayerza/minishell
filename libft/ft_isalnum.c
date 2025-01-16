/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.barcelona42.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:07:23 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/16 18:32:32 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9'));
}
/*
int	main(void)
{
	#include <stdio.h>

	printf("res: %d\n", ft_isalnum('s'));
	printf("res: %d\n", ft_isalnum('S'));
	printf("res: %d\n", ft_isalnum('4'));
	printf("res: %d\n", ft_isalnum(' '));
	printf("res: %d\n", ft_isalnum('\n'));
	return (0);
}*/
