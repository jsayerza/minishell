#include "../minishell.h"

void	env(t_constructor *node)
{
	if (node->size == 0)
		print_env(node->shell);
}
