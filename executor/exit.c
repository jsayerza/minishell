#include "../minishell.h"

static bool is_numeric(const char *str)
{
	int i = 0;

	if (!str || !*str)
		return false;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!isdigit(str[i]))
			return false;
		i++;
	}
	return true;
}

void process_exit(t_constructor *node)
{
	long long exit_code;
	int final_exit_code;

	if (node->size_exec > 2)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		node->shell->last_exit = 1;
		return;
	}
	if (node->size_exec == 2)
	{
		if (!is_numeric(node->executable[1]))
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(node->executable[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			node->shell->last_exit = 2;
			exit(2);
		}
		exit_code = atoll(node->executable[1]);
		// Manejar overflow/underflow usando módulo 256 para simular comportamiento de 8 bits
		final_exit_code = (int)(exit_code % 256);
		if (final_exit_code < 0)
			final_exit_code += 256;
		
		node->shell->last_exit = final_exit_code;
		exit(final_exit_code);
	}
	// Exit sin argumentos - usar el último exit code
	exit(node->shell->last_exit);
}