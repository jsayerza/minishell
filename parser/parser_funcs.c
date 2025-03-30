/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 11:40:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ast_print(t_ast *root, int depth)
{
	if (!root)
		return;

	// Indentation for tree structure
	for (int i = 0; i < depth; i++)
		printf("  ");

	// Print node type
	switch (root->type)
	{
		case TOKEN_COMMAND:
			printf("CMD: ");
			for (int i = 0; root->args && root->args[i]; i++)
				printf("%s ", root->args[i]);
			printf("\n");
			break;
		case TOKEN_PIPE:
			printf("PIPE\n");
			break;
		case TOKEN_REDIRECT_IN:
			printf("REDIRECT_IN: %s\n", root->file);
			break;
		case TOKEN_REDIRECT_OUT:
			printf("REDIRECT_OUT: %s\n", root->file);
			break;
		case TOKEN_HEREDOC:
			printf("HEREDOC: %s\n", root->file);
			break;
		case TOKEN_APPEND:
			printf("APPEND: %s\n", root->file);
			break;
		default:
			printf("UNKNOWN NODE\n");
	}

	// Recursively print left and right subtrees
	if (root->left)
	{
		for (int i = 0; i < depth; i++)
			printf("  ");
		printf("LEFT →\n");
		ast_print(root->left, depth + 1);
	}
	if (root->right)
	{
		for (int i = 0; i < depth; i++)
			printf("  ");
		printf("RIGHT →\n");
		ast_print(root->right, depth + 1);
	}
}
