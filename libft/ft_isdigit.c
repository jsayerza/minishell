/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.barcelona42.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:07:23 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/16 18:51:00 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}
/*
int	main(void)
{
	#include <stdio.h>

	printf("res: %d\n", ft_isdigit('s'));
	printf("res: %d\n", ft_isdigit('S'));
	printf("res: %d\n", ft_isdigit('4'));
	printf("res: %d\n", ft_isdigit(' '));
	printf("res: %d\n", ft_isdigit('\n'));
	return (0);
}*/
