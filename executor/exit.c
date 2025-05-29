#include "../minishell.h"

static bool is_numeric(const char *str)
{
	int i = 0;

	if (!str || !*str)
		return false;
	if (str[i] == '-' || str[i] == '+')
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

	if (node->size_exec > 2)
	{
		printf("exit: too many arguments\n");
		node->shell->last_exit = 2;
		return;
	}
	if (node->size_exec == 2)
	{
		if (!is_numeric(node->executable[1]))
		{
			printf("exit: %s: numeric argument required\n", node->executable[1]);
			node->shell->last_exit = 2;
			exit(2);
		}
		exit_code = atoll(node->executable[1]);
		if (exit_code > 255 || exit_code < 0)
		{
			printf("exit: %s: numeric argument required\n", node->executable[1]);
			node->shell->last_exit = 2;
			exit(2);
		}
		node->shell->last_exit = exit_code;
		exit(exit_code);
	}
	printf("exit\n");
	exit(node->shell->last_exit);
}
