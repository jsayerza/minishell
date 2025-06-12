#include "../minishell.h"

void	check_redirect_in_file_exists(t_const *node)
{
	t_const	*current;
	int		fd;
	int		i;

	if (!node)
		return ;
	current = node;
	if (!current->redirect_in)
		return ;
	i = 0;
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

void	create_output_files(t_const *node)
{
	int	fd;
	int	i;

	if (!node || !node->redirect_out)
		return ;
	i = 0;
	while (node->redirect_out[i])
	{
		fd = open(node->redirect_out[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("open");
			return ;
		}
		close(fd);
		i++;
	}
}

void	create_append_files(t_const *node)
{
	int	fd;
	int	i;

	if (!node || !node->redirect_append)
		return ;
	i = 0;
	while (node->redirect_append[i])
	{
		fd = open(node->redirect_append[i], O_WRONLY
				| O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("open");
			return ;
		}
		close(fd);
		i++;
	}
}

int	validate_all_input_redirections(t_const *node)
{
	int	fd;
	int	i;

	if (!node || !node->redirect_in)
		return (1);
	i = 0;
	while (node->redirect_in[i])
	{
		fd = open(node->redirect_in[i], O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(node->redirect_in[i], STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			return (0);
		}
		close(fd);
		i++;
	}
	return (1);
}

int	validate_output_permissions_no_delete(char **files)
{
	int	fd;
	int	i;

	if (!files)
		return (1);
	i = 0;
	while (files[i])
	{
		fd = open(files[i], O_WRONLY | O_CREAT, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(files[i], STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			return (0);
		}
		close(fd);
		i++;
	}
	return (1);
}

int	validate_append_permissions_no_delete(char **files)
{
	int	fd;
	int	i;

	if (!files)
		return (1);
	i = 0;
	while (files[i])
	{
		fd = open(files[i], O_WRONLY | O_CREAT, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(files[i], STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			return (0);
		}
		close(fd);
		i++;
	}
	return (1);
}

int	validate_all_output_redirections_no_delete(t_const *node)
{
	if (!node)
		return (1);
	if (!validate_output_permissions_no_delete(node->redirect_out))
		return (0);
	if (!validate_append_permissions_no_delete(node->redirect_append))
		return (0);
	return (1);
}

int	validate_file_array(char **files, int flags)
{
	int	fd;
	int	i;

	if (!files)
		return (1);
	i = 0;
	while (files[i])
	{
		fd = open(files[i], flags, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(files[i], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			return (0);
		}
		close(fd);
		i++;
	}
	return (1);
}

int	validate_all_output_redirections(t_const *node)
{
	if (!node)
		return (1);
	if (!validate_file_array(node->redirect_out, O_WRONLY
			| O_CREAT | O_TRUNC))
		return (0);
	if (!validate_file_array(node->redirect_append, O_WRONLY
			| O_CREAT | O_APPEND))
		return (0);
	return (1);
}
