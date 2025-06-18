/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 export.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:14:08 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:14:08 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void	add_to_env(char ***env, char *var)
{
	int		len;
	int		index;
	char	**new_env;

	if (var_value(var) == -1)
		return ;
	len = 0;
	while ((*env)[len])
		len++;
	index = find_in_env(*env, var);
	if (update_existing_var(env, var, index))
		return ;
	new_env = create_new_env(len);
	if (!new_env)
		return ;
	copy_elements(new_env, *env, len);
	add_new_element(new_env, var, len);
	free(*env);
	*env = new_env;
}

void	update_env_var(t_const *node, char *arg, int index_env)
{
	if (index_env != -1)
	{
		free(node->shell->env[index_env]);
		node->shell->env[index_env] = strdup(arg);
	}
	else
	{
		add_to_env(&(node->shell->env), arg);
	}
}

void	handle_process_export(t_const *node, char *arg,
		int index_env, int index_export)
{
	char	*new_var;

	if (index_env != -1)
	{
		new_var = process_value(NULL, node->shell->env[index_env]);
		update_export_var(node, new_var, index_export);
		free(new_var);
	}
	else
	{
		if (index_export == -1)
			add_to_export(&(node->shell->export), arg);
	}
}

void	process_export_var(t_const *node, char *arg)
{
	int		index_env;
	int		index_export;
	char	*var_name;
	char	*new_var;

	var_name = extract_var_name(arg);
	index_env = find_in_env(node->shell->env, var_name);
	index_export = find_in_export(node->shell->export, var_name);
	if (var_value(arg) == 1)
	{
		update_env_var(node, arg, index_env);
		new_var = process_value(NULL, arg);
		update_export_var(node, new_var, index_export);
		free(new_var);
	}
	else
		handle_process_export(node, arg, index_env, index_export);
	free(var_name);
	sort_export(node->shell);
}

void	export(t_const *node)
{
	int	j;

	if (node->pipe_out)
		redirect_builtin(node, node->shell->export);
	else if (!node->executable[1])
		print_builtin(node->shell->export);
	else
	{
		j = 1;
		while (j < node->size_exec)
		{
			if (!is_valid_identifier(node->executable[j]))
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(node->executable[j], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				node->shell->last_exit = 1;
			}
			else
				process_export_var(node, node->executable[j]);
			j++;
		}
	}
}
