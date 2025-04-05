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

void	cd_home(t_constructor *node)
{
	char	*home;

	home = get_home(node->shell);
	printf("%s\n", home);
	chdir(home);
	refresh_directori(node->shell, home, 1);
}

void	cd_oldpwd(t_constructor *node)
{
	char	*pwd;

	if (!node->shell->oldpwd)
	{
		printf("cd: OLDPWD not set\n");
		return ;
	}
	pwd = malloc((ft_strlen(node->shell->oldpwd) + 1) * sizeof(char));
	if (!pwd)
		return ;
	strcpy(pwd, node->shell->oldpwd);
	if (chdir(pwd) == 0)
		refresh_directori(node->shell, pwd, 1);
	else
	{
		printf("cd: %s: No such file or directory\n", pwd);
		free(pwd);
	}
}

void	cd_directori(t_constructor *node)
{
	if (chdir(node->executable[1]) == -1)
	{
		node->error = ERROR_NOT_ACCESS;
		return ;
	}
	refresh_directori(node->shell, node->executable[1], 0);
}

void	cd(t_constructor *node)
{
	if (node->size_exec == 1)
		cd_home(node);
	else if (node->size_exec == 2
		&& ft_strncmp(node->executable[1], "-", 1) == 0)
		cd_oldpwd(node);
	else
		cd_directori(node);
}
