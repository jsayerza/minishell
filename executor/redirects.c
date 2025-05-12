#include "../minishell.h"

int check_redirect_in_file_exists(t_constructor *node)
{
	t_constructor *current;
	int fd;

	if (!node)
		return 0;

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
				return 1; // Error al abrir el archivo
			}
			close(fd); // Solo verificamos que se pueda abrir
		}
		current = current->next;
	}
	return 0; // Todo correcto
}

void apply_all_redirections(t_constructor *node)
{
	t_constructor *current;
	int fd;

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
				exit(1);
			}
			if (dup2(fd, STDIN_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				exit(1);
			}
			close(fd);
		}
		current = current->next;
	}
}
