#include "../minishell.h"

void check_redirect_in(t_constructor *node)
{
	t_constructor *current;
	int fd;

	if (!node)
		return;

	current = node;
	while (current && current->prev)
		current = current->prev;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_IN && current->file)
		{
			fd = open(current->file, O_RDONLY);
			if (fd < 0)
			{
				perror("open");
				return;
			}
			if (dup2(fd, STDIN_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				return;
			}
			close(fd);
		}
		current = current->next;
	}
}
