/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/01/16 17:44:28 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Acarranz

int	main(int ac, char **av)
{
	for (int i = 0; i < ac; i++)
	{
		printf("av[%d]:%s\n", i, av[i]);
	}
	printf("num:%d\n", ft_atoi(av[3]));
	return (EXIT_SUCCESS);
}
