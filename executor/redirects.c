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
			perror("open");
			return ;
		}
		close(fd);
		i++;
	}
}

void create_output_files(t_constructor *node)
{
	int fd;
	int i;

	if (!node || !node->redirect_out)
		return;

	i = 0;
	while (node->redirect_out[i])
	{
		if (node->redirect_out[i + 1] != NULL)
		{
			fd = open(node->redirect_out[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("open");
				return;
			}
			close(fd);
		}
		i++;
	}
}

void create_append_files(t_constructor *node)
{
	int fd;
	int i;

	if (!node || !node->redirect_append)
		return;

	i = 0;
	while (node->redirect_append[i])
	{
		// For append files that aren't the last, ensure they exist but don't write to them
		if (node->redirect_append[i + 1] != NULL)
		{
			fd = open(node->redirect_append[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				perror("open");
				return;
			}
			close(fd);
		}
		i++;
	}
}

void apply_input_redirection(t_constructor *node)
{
	int fd;
	int i;

	if (!node || !node->redirect_in)
		return;

	i = 0;
	while (node->redirect_in[i])
	{
		fd = open(node->redirect_in[i], O_RDONLY);
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

void apply_output_redirection(t_constructor *node)
{
	int fd;
	int i;

	if (!node || !node->redirect_out)
		return;
	i = 0;
	while (node->redirect_out[i] && node->redirect_out[i + 1])
		i++;

	if (node->redirect_out[i])
	{
		fd = open(node->redirect_out[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("open");
			exit(1);
		}
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2");
			close(fd);
			exit(1);
		}
		close(fd);
	}
}

void apply_append_redirection(t_constructor *node)
{
	int fd;
	int i;

	if (!node || !node->redirect_append)
		return;
	i = 0;
	while (node->redirect_append[i] && node->redirect_append[i + 1])
		i++;
	if (node->redirect_append[i])
	{
		fd = open(node->redirect_append[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("open");
			exit(1);
		}
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2");
			close(fd);
			exit(1);
		}
		close(fd);
	}
}

void apply_all_redirections(t_constructor *node)
{
	if (!node)
		return;
	create_output_files(node);
	create_append_files(node);
	apply_input_redirection(node);
	apply_output_redirection(node);
	if (!node->redirect_out || !node->redirect_out[0])
		apply_append_redirection(node);
}
