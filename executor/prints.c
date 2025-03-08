#include "../minishell.h"

void print_env(t_shell *shell)
{
    printf("Print Env\n");
	int	i;

	i = 0;
	while (shell->env[i])
		printf("%s\n", shell->env[i++]);
}

void print_export(t_shell *shell)
{
    printf("Print Export\n");
	int	i;


	i = 0;
	while (shell->export[i] != NULL)
		printf("%s\n", shell->export[i++]);
}


void print_constructor(t_shell *shell) {
    if (!shell || !shell->constructor) {
        fprintf(stderr, "Error: Constructor no inicializado.\n");
        return;
    }

    t_constructor *current = shell->constructor;
    int node_number = 0;

    while (current) {
        printf("\n----- Nodo %d -----\n", node_number);
        printf("Tipo de token: %d\n", current->type);
        printf("Tipo de builtin: %d\n", current->builtin);
        printf("Estado de error: %d\n", current->error);
        printf("Ejecutables: ");
        for (int i = 0; current->executable[i]; i++) {
            printf("\"%s\" ", current->executable[i]);
        }
        printf("\nPaths: ");
        if (current->paths) {
            for (int i = 0; current->paths[i]; i++) {
                printf("\"%s\" ", current->paths[i]);
            }
        } else {
            printf("NULL");
        }
        printf("\n========================\n");

        current = current->next;
        node_number++;
    }
}
