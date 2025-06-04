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
        printf("cd: HOME not set\n");
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
    const char *oldpwd = get_env_value(node->shell, "OLDPWD");

    if (!oldpwd)
    {
        printf("cd: OLDPWD not set\n");
        node->shell->last_exit = 1;
        return;
    }

    printf("%s\n", oldpwd);
    if (chdir(oldpwd) == -1)
    {
        printf("cd: %s: No such file or directory\n", oldpwd);
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
        printf("cd: %s: No such file or directory\n", target_path);
        node->shell->last_exit = 1;
        return;
    }

    // Determinar si es ruta absoluta o relativa
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
        printf("cd: too many arguments\n");
        node->shell->last_exit = 1;
    }
}
