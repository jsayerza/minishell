#include "../minishell.h"

void    read_fd(t_constructor *node)
{
    node->fd[0] = open(node->executable[0], O_RDONLY);
    close(node->fd[1]);
    dup2(node->fd[0], STDOUT_FILENO);
    close(node->fd[0]);
}