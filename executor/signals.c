#include "../minishell.h"
#include "../minishell.h"
#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

// Manejador de señales para SIGINT (Ctrl+C) y SIGQUIT (Ctrl+\)
void signal_handler_with_context(int sig, siginfo_t *info, void *context)
{
	(void)info;
	t_shell *shell = (t_shell *)context;

	if (sig == SIGINT) // Ctrl+C
	{
		shell->last_exit = 130; // Código de salida para Ctrl+C
		printf("\n");			// Imprime una nueva línea
		rl_on_new_line();
		rl_replace_line("", 0); // Limpia la línea actual
		rl_redisplay();			// Muestra un nuevo prompt
	}
	else if (sig == SIGQUIT) // Ctrl+
	{
		shell->last_exit = 131; // Código de salida para Ctrl+
		return;					// No hace nada
	}
}

// Configura las señales para el shell
void setup_signals(t_shell *shell)
{
	(void)shell;
	struct sigaction sa;

	// Configura el manejador de señales
	memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = signal_handler_with_context;
	sa.sa_flags = SA_SIGINFO; // Permite pasar el contexto

	// Configura SIGINT (Ctrl+C)
	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("Error configurando SIGINT");

	// Configura SIGQUIT (Ctrl+\)
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		perror("Error configurando SIGQUIT");

	// Ignora SIGTERM
	signal(SIGTERM, SIG_IGN);
}

// Maneja señales dentro del bucle principal
void handle_signals_in_loop(t_shell *shell, char *line)
{
	if (!line) // Ctrl+D
	{
		printf("exit\n");
		free_path_array(shell->paths);
		free_path_array(shell->env);
		free(shell->path);
		collector_cleanup(&shell->collector);
		exit(shell->last_exit); // Salir del programa con el último código de salida
	}

	if (shell->last_exit == 130) // Ctrl+C
	{
		shell->last_exit = 0; // Restablece el estado después de manejar Ctrl+C
		printf("\n");		  // Imprime una nueva línea
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}

	if (shell->last_exit == 131) // Ctrl+
	{
		shell->last_exit = 0; // Restablece el estado después de manejar Ctrl+
		printf("Quit: 3\n");  // Mensaje similar al comportamiento de bash
	}
}
