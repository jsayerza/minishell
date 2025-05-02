#include "../minishell.h"

static void clean_executable(t_constructor *current)
{
    int i;

    if (!current || !current->executable)
        return;
    
    i = 0;
    while (current->executable[i])
        free(current->executable[i++]);
    free(current->executable);
    current->executable = NULL;
}

void clean_constructor(t_constructor *constructor)
{
    t_constructor *current;
    t_constructor *next;

    if (!constructor)
        return;
    
    current = constructor;
    while (current)
    {
        next = current->next;
        clean_executable(current);
        free(current);
        current = next;
    }
}

void clean_paths(t_shell *shell)
{
    if (!shell)
        return;
    
    if (shell->path)
    {
        free(shell->path);
        shell->path = NULL;
    }
    
    if (shell->paths)
    {
        free_path_array(shell->paths);
        shell->paths = NULL;
    }
}

void clean_shell(t_shell *shell)
{
    int i;

    if (!shell)
        return;
    
    // Limpiar environment
    if (shell->env)
    {
        i = 0;
        while (shell->env[i])
            free(shell->env[i++]);
        free(shell->env);
        shell->env = NULL;
    }
    
    // Limpiar export
    if (shell->export)
    {
        i = 0;
        while (shell->export[i])
            free(shell->export[i++]);
        free(shell->export);
        shell->export = NULL;
    }
    
    // Limpiar paths
    clean_paths(shell);
    
    // Limpiar otros campos
    free(shell->pwd);
    free(shell->oldpwd);
    free(shell->output);
    
    // Limpiar constructor
    clean_constructor(shell->constructor);
    
    // Finalmente liberar la estructura shell
    free(shell);
}