/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.barcelona42.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:07:23 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/16 18:32:32 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}
/*
int	main(void)
{
	#include <stdio.h>

	printf("res: %d\n", ft_isalpha('s'));
	printf("res: %d\n", ft_isalpha('S'));
	printf("res: %d\n", ft_isalpha('4'));
	printf("res: %d\n", ft_isalpha(' '));
	printf("res: %d\n", ft_isalpha('\n'));
	return (0);
}*/
