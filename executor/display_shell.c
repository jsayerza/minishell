#include "../minishell.h"

void display_shell(t_shell *shell)
{
	t_constructor	*current_node;

	current_node = shell->constructor;

	while(current_node)
	{
		current_node->shell = shell;
		if (current_node->type == TOKEN_BUILTIN)
			token_builtins(current_node);
		current_node = current_node->next;
	}
}
