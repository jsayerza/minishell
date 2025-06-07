/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 cd.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/04/03 20:02:57 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/04/03 20:02:57 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void cd_home(t_constructor *node)
{
    const char *home = get_env_value(node->shell, "HOME");

    if (!home)
    {
        ft_putstr_fd("cd: HOME not set\n", 2);
        node->shell->last_exit = 1;
        return;
    }

    if (chdir(home) == -1)
    {
        perror("cd");
        node->shell->last_exit = 1;
        return;
    }

    refresh_directori(node->shell, home, 1);
}

void cd_oldpwd(t_constructor *node)
{
    char    *oldpwd = get_env_value(node->shell, "OLDPWD");

    if (!oldpwd)
    {
        ft_putstr_fd("cd: OLDPWD not set\n", 2);
        node->shell->last_exit = 1;
        return;
    }

    if (chdir(oldpwd) == -1)
    {
        ft_putstr_fd("cd: ", 2);
        ft_putstr_fd(oldpwd, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        node->shell->last_exit = 1;
        return;
    }

    refresh_directori(node->shell, oldpwd, 1);
}

void cd_directori(t_constructor *node)
{
    char *target_path = node->executable[1];

    if (chdir(target_path) == -1)
    {
        ft_putstr_fd("cd: ", 2);
        ft_putstr_fd(target_path, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        node->shell->last_exit = 1;
        return;
    }
    int is_absolute = (target_path[0] == '/');
    refresh_directori(node->shell, target_path, is_absolute);
}

void cd(t_constructor *node)
{
    if (node->size_exec == 1)
        cd_home(node);
    else if (node->size_exec == 2 && ft_strncmp(node->executable[1], "-", 2) == 0)
        cd_oldpwd(node);
    else if (node->size_exec == 2)
        cd_directori(node);
    else
    {
        ft_putstr_fd("cd: too many arguments\n", 2);
        node->shell->last_exit = 1;
    }
}
