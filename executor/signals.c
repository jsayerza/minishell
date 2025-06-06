/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 12:00:00 by acarranz          #+#    #+#             */
/*   Updated: 2025/06/06 12:00:00 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Variable global para acceder al shell desde el handler

t_shell *g_shell = NULL;

// Handler para el proceso padre (minishell)
void signal_handler(int sig)
{
    if (sig == SIGINT)  // Ctrl+C
    {
        printf("\n");
        rl_on_new_line();      // Readline: nueva línea
        rl_replace_line("", 0); // Limpiar línea actual
        rl_redisplay();        // Mostrar prompt nuevamente
        if (g_shell)
            g_shell->last_exit = 130; // Exit code para SIGINT
    }
    // SIGQUIT (Ctrl+\) se ignora en modo interactivo
}

// Handler para procesos hijos
void signal_handler_child(int sig)
{
    if (sig == SIGINT)  // Ctrl+C
    {
        printf("\n");
        if (g_shell)
            g_shell->last_exit = 130;
    }
    else if (sig == SIGQUIT)  // Ctrl+backslash
    {
        printf("Quit: 3\n");
        if (g_shell)
            g_shell->last_exit = 131;
    }
}

// Configurar señales para el proceso padre
void setup_signals(void)
{
    signal(SIGINT, signal_handler);   // Ctrl+C
    signal(SIGQUIT, SIG_IGN);         // Ignorar Ctrl+\ en shell
}

// Configurar señales para procesos hijos
void setup_child_signals(void)
{
    signal(SIGINT, signal_handler_child);   // Ctrl+C
    signal(SIGQUIT, signal_handler_child);  // Ctrl+
}