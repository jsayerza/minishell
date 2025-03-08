#include "../minishell.h"

void	env(t_constructor *node)
{
	if (node->shell->node_size == 1)
		print_env(node->shell);
}
