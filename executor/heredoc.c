#include "../minishell.h"

void 	check_heredoc(t_constructor *node)
{
	t_constructor	*current;
	int				i;

	if (!node->heredoc)
		return ;
	i = 0;
	current = node;
	while (current->heredoc[i])
		i++;
	if (dup2(current->heredoc[-i], STDIN_FILENO) < 0)
	{
		perror("Error heredoc");
		exit (1);
	}
}
