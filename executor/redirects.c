/* ************************************************************************** */
/*                                                         */
/*                                                         :::       ::::::::   */
/*       redirects.c                                     :+:               :+:    :+:   */
/*                                                         +:+ +:+                  +:+     */
/*       By: acarranz <marvin@42.fr>                     +#+  +:+    +#+            */
/*                                                         +#+#+#+#+#+    +#+                       */
/*       Created: 2025/03/23 12:13:24 by acarranz           #+#     #+#                     */
/*       Updated: 2025/06/11 XX:XX:XX by acarranz          ###    ########.fr               */
/*                                                         */
/* ************************************************************************** */

#include "../minishell.h"

void    apply_input_redirection(t_const *node)
{
        int     fd;
        int     i;

        if (!node || !node->redirect_in)
                return ;
        i = 0;
        while (node->redirect_in[i])
        {
                fd = open(node->redirect_in[i], O_RDONLY);
                if (fd < 0)
                {
                        perror("open");
                        exit(1);
                }
                if (dup2(fd, STDIN_FILENO) < 0)
                {
                        perror("dup2");
                        close(fd);
                        exit(1);
                }
                close(fd);
                i++;
        }
}

void apply_output_redirection(t_const *node)
{
    int fd;
    int i;

    if (!node || !node->redirect_out)
    {
        printf("DEBUG: No hay redirección de salida\n");
        return ;
    }
    
    printf("DEBUG: Iniciando redirección de salida\n");
    
    i = 0;
    while (node->redirect_out[i] && node->redirect_out[i + 1])
        i++;
    
    if (node->redirect_out[i])
    {
        printf("DEBUG: Abriendo archivo: %s\n", node->redirect_out[i]);
        
        fd = open(node->redirect_out[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
            printf("DEBUG: Error abriendo archivo: %s\n", strerror(errno));
            perror("open");
            exit(1);
        }
        
        printf("DEBUG: Archivo abierto con fd: %d\n", fd);
        
        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            printf("DEBUG: Error en dup2: %s\n", strerror(errno));
            perror("dup2");
            close(fd);
            exit(1);
        }
        
        printf("DEBUG: dup2 exitoso, cerrando fd original\n");
        close(fd);
        
        printf("DEBUG: Redirección aplicada correctamente\n");
    }
}

void    apply_append_redirection(t_const *node)
{
        int     fd;
        int     i;

        if (!node || !node->redirect_append)
                return ;
        i = 0;
        while (node->redirect_append[i] && node->redirect_append[i + 1])
                i++;
        if (node->redirect_append[i])
        {
                fd = open(node->redirect_append[i], O_WRONLY
                                | O_CREAT | O_APPEND, 0644);
                if (fd < 0)
                {
                        perror("open");
                        exit(1);
                }
                if (dup2(fd, STDOUT_FILENO) < 0)
                {
                        perror("dup2");
                        close(fd);
                        exit(1);
                }
                close(fd);
        }
}

void    apply_all_redirections(t_const *node)
{
        if (!node)
                return ;

        // Aplicar redirecciones de entrada primero
        apply_input_redirection(node);

        // Aplicar redirecciones de salida (pueden sobrescribir pipes)
        if (node->redirect_out_type == TOKEN_REDIRECT_OUT)
                apply_output_redirection(node);
        else if (node->redirect_out_type == TOKEN_APPEND)
                apply_append_redirection(node);
}