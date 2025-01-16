/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.barcelona42.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:07:23 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/16 18:51:00 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	return (c >= 32 && c <= 126);
}
/*
int	main(void)
{
	#include <stdio.h>

	printf("res: %d\n", ft_isprint('s'));
	printf("res: %d\n", ft_isprint('S'));
	printf("res: %d\n", ft_isprint('4'));
	printf("res: %d\n", ft_isprint(' '));
	printf("res: %d\n", ft_isprint('\n'));
	return (0);
}*/
