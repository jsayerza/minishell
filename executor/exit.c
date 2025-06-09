/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 exit.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:24 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:24 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (false);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

static void	handle_too_many_args(t_constructor *node)
{
	ft_putstr_fd("exit: too many arguments\n", 2);
	node->shell->last_exit = 1;
}

static void	handle_non_numeric_arg(t_constructor *node)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(node->executable[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	node->shell->last_exit = 2;
	exit(2);
}

static void	handle_numeric_arg(t_constructor *node)
{
	long long	exit_code;
	int			final_exit_code;

	exit_code = atoll(node->executable[1]);
	final_exit_code = (int)(exit_code % 256);
	if (final_exit_code < 0)
		final_exit_code += 256;
	node->shell->last_exit = final_exit_code;
	exit(final_exit_code);
}

void	process_exit(t_constructor *node)
{
	if (node->size_exec > 2)
	{
		handle_too_many_args(node);
		return ;
	}
	if (node->size_exec == 2)
	{
		if (!is_numeric(node->executable[1]))
		{
			handle_non_numeric_arg(node);
		}
		handle_numeric_arg(node);
	}
	exit(node->shell->last_exit);
}
