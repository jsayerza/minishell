/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/06/07 08:02:34 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void update_shlvl(t_shell *shell, int increment)
{
    char *shlvl_str;
    int shlvl;
    char *new_shlvl;

    if (!shell)
        return;

    shlvl_str = get_env_value(shell, "SHLVL");
    if (shlvl_str)
        shlvl = ft_atoi(shlvl_str);
    else
        shlvl = 0;

    shlvl += increment;
    if (shlvl < 0)
        shlvl = 0;

    new_shlvl = ft_itoa(shlvl);
    if (!new_shlvl)
        return;
    add_or_update_env_var(&shell->env, "SHLVL", new_shlvl);
    add_or_update_env_var(&shell->export, "declare -x SHLVL", new_shlvl);
    free(new_shlvl);
}

void cleanup_shell(t_shell *shell)
{
    if (!shell)
        return;

    if (shell->paths)
    {
        free_path_array(shell->paths);
        shell->paths = NULL;
    }
    if (shell->env)
    {
        free_path_array(shell->env);
        shell->env = NULL;
    }
    if (shell->export)
    {
        free_path_array(shell->export);
        shell->export = NULL;
    }
    if (shell->path)
    {
        free(shell->path);
        shell->path = NULL;
    }
}

int	main(int argc, char **argv, char **envp)
{
	t_collector		*collector;
	t_collector		*cycle_collector;
	t_token			*tokens;
	t_shell			*shell;
	t_ast			*ast;
	t_constructor	*constructor;
	char			*line;
	char			*prompt;

	(void)argc;
	(void)argv;
	collector = NULL;
	shell = init_shell(NULL, envp, &collector);
	if (!shell)
		exit_program(&collector, "Error al inicializar shell", true);
		 g_shell = shell;
    setup_signals();
	update_shlvl(shell, 1);
	while (1)
	{

		cycle_collector = NULL;
		line = NULL;
		prompt = prompt_generate(&cycle_collector);
		line = readline(prompt);
		if (!line)
		{
			collector_cleanup(&cycle_collector);
			break ;
		}
		if (!line)
			break ;
		if (line[0] == '\0' || is_only_whitespace(line))
		{
			freer(line);
			collector_cleanup(&cycle_collector);
			continue ;
		}
		if (has_unclosed_quotes(line))
		{
			print_error("minishell: unclosed quotes");
			freer(line);
			collector_cleanup(&cycle_collector);
			continue ;
		}
		add_history(line);
		tokens = NULL;
		tokens = lexer(line, &cycle_collector, &tokens, shell);
		freer(line);
		if (!tokens)
		{
			collector_cleanup(&cycle_collector);
			continue ;
		}
		ast = parser(&cycle_collector, tokens);
		if (!ast)
		{
			collector_cleanup(&cycle_collector);
			continue ;
		}
		constructor = ast_to_constructor(&cycle_collector, ast, shell);
		shell->constructor = constructor;
		if (constructor)
		{
			//constructor_print(constructor);
			//ft_putstr_fd("\n=== Execute shell ===\n");
			display_shell(shell);
		}
		else
			print_error("minishell: failed to prepare command execution");
		collector_cleanup(&cycle_collector);
	}
	cleanup_shell(shell);
	collector_cleanup(&collector);
	return (EXIT_SUCCESS);
}
