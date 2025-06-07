/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 11:40:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/22 17:45:41 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ast_print_indent(int depth)
{
	int	i;

	i = 0;
	while (i++ < depth)
		printf("  ");
}

static void	ast_print_branch(char *label, t_ast *child, int depth)
{
	ast_print_indent(depth);
	printf(CYAN "%s →\n" RESET, label);
	if (child)
		ast_print(child, depth + 1);
	else
	{
		ast_print_indent(depth + 1);
		printf("[NULL]\n");
	}
}

void	ast_print(t_ast *root, int depth)
{
	if (!root)
		return ;
	ast_print_type(root, depth);
	ast_print_branch("LEFT", root->left, depth);
	ast_print_branch("RIGHT", root->right, depth);
}
