#include "../minishell.h"

void	check_redirect_in_file_exists(t_constructor *node)
{
	t_constructor *current;
	int fd;

	if (!node)
		return ;
	current = node;
	if (!current->redirect_in)
		return ;
	int i = 0;
	while (current->redirect_in[i])
	{
		fd = open(current->redirect_in[i], O_RDONLY);
		if (fd < 0)
		{
			perror("open"); // Error al abrir el archivo
			return ;
		}
		close(fd);
		i++;
	}
}

void apply_all_redirections(t_constructor *node)
{
	t_constructor *current;
	int fd;

	current = node;
	if (!current->redirect_in)
		return ;
	int i = 0;
	while (current->redirect_in[i])
	{

		fd = open(current->redirect_in[i], O_RDONLY);
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
		i++;
	}
}
