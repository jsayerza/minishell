/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.barcelona42.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:07:23 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/16 18:51:00 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	return (c >= 0 && c <= 127);
}
/*
int	main(void)
{
	#include <stdio.h>

	printf("res: %d\n", ft_isascii('s'));
	printf("res: %d\n", ft_isascii('S'));
	printf("res: %d\n", ft_isascii('4'));
	printf("res: %d\n", ft_isascii(' '));
	printf("res: %d\n", ft_isascii('\n'));
	return (0);
}*/
