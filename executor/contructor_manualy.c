#include "../minishell.h"

t_constructor *create_single_constructor_node(int node_number)
{
    t_constructor *new_node = (t_constructor *)malloc(sizeof(t_constructor));
    if (!new_node)
    {
        perror("malloc");
        return NULL;
    }

    new_node->executable = NULL;
    new_node->size_exec = 0;
    new_node->builtin = BUILTIN_NONE;
    new_node->type = TOKEN_COMMAND;
    new_node->error = NO_ERROR;
    new_node->next = NULL;
    new_node->pipe_in = -1;  // Initialize pipe_in to 0
    new_node->pipe_out = -1; // Initialize pipe_out to 0
    
    printf("\n----- Nodo %d -----\n", node_number);
    printf("Ingrese el tipo de token (0-13):\n");
    printf("0: TOKEN_EOF\n1: TOKEN_WORD\n2: TOKEN_PIPE\n3: TOKEN_REDIRECT_IN\n");
    printf("4: TOKEN_REDIRECT_OUT\n5: TOKEN_APPEND\n6: TOKEN_HEREDOC\n7: TOKEN_AND\n");
    printf("8: TOKEN_OR\n9: TOKEN_WILDCARD\n10: TOKEN_DOLLAR\n11: TOKEN_ESCAPE\n");
    printf("12: TOKEN_COMMAND\n13: TOKEN_BUILTIN\n");
    int token_type;
    if (scanf("%d", &token_type) != 1 || token_type < 0 || token_type > 13)
    {
        fprintf(stderr, "Error: Tipo de token no válido.\n");
        free(new_node);
        return NULL;
    }
    getchar();
    new_node->type = (t_token_type)token_type;
    printf("Token type entered: %d\n", token_type); // Depuración
    printf("Node type assigned: %d\n", new_node->type); // Depuración
    
    if (new_node->type == TOKEN_BUILTIN)
    {
        printf("Ingrese el tipo de builtin (0-7):\n");
        printf("0: BUILTIN_NONE\n1: BUILTIN_ECHO\n2: BUILTIN_CD\n3: BUILTIN_PWD\n");
        printf("4: BUILTIN_EXPORT\n5: BUILTIN_UNSET\n6: BUILTIN_ENV\n7: BUILTIN_EXIT\n");
        int builtin;
        if (scanf("%d", &builtin) != 1 || builtin < 0 || builtin > 7)
        {
            fprintf(stderr, "Error: Tipo de builtin no válido.\n");
            free(new_node);
            return NULL;
        }
        getchar();
        new_node->builtin = (t_builtin)builtin;
        printf("Builtin type entered: %d\n", builtin); // Depuración
        printf("Node builtin assigned: %d\n", new_node->builtin); // Depuración
    }
    
    // Get pipe_in value
    printf("Ingrese el valor de pipe_in (0 si no existe): ");
    int pipe_in;
    if (scanf("%d", &pipe_in) != 1 || pipe_in < 0)
    {
        fprintf(stderr, "Error: Valor de pipe_in no válido.\n");
        free(new_node);
        return NULL;
    }
    getchar();
    new_node->pipe_in = pipe_in;
    printf("Pipe_in value entered: %d\n", pipe_in); // Depuración
    
    // Get pipe_out value
    printf("Ingrese el valor de pipe_out (0 si no existe): ");
    int pipe_out;
    if (scanf("%d", &pipe_out) != 1 || pipe_out < 0)
    {
        fprintf(stderr, "Error: Valor de pipe_out no válido.\n");
        free(new_node);
        return NULL;
    }
    getchar();
    new_node->pipe_out = pipe_out;
    printf("Pipe_out value entered: %d\n", pipe_out); // Depuración
    
    printf("¿Cuántos ejecutables desea ingresar? ");
    int executable_count;
    if (scanf("%d", &executable_count) != 1 || executable_count < 0)
    {
        fprintf(stderr, "Error: Número de ejecutables no válido.\n");
        free(new_node);
        return NULL;
    }
    getchar();
    new_node->executable = (char **)malloc((executable_count + 1) * sizeof(char *));
    if (!new_node->executable)
    {
        perror("malloc");
        free(new_node);
        return NULL;
    }
    for (int i = 0; i < executable_count; i++)
    {
        char executable[1024];
        printf("Ingrese el ejecutable %d: ", i + 1);
        if (!fgets(executable, sizeof(executable), stdin))
        {
            fprintf(stderr, "Error al leer el ejecutable.\n");
            for (int j = 0; j < i; j++)
            {
                free(new_node->executable[j]);
            }
            free(new_node->executable);
            free(new_node);
            return NULL;
        }
        executable[strcspn(executable, "\n")] = 0;
        new_node->executable[i] = strdup(executable);
        if (!new_node->executable[i])
        {
            perror("strdup");
            for (int j = 0; j < i; j++)
            {
                free(new_node->executable[j]);
            }
            free(new_node->executable);
            free(new_node);
            return NULL;
        }
    }
    new_node->executable[executable_count] = NULL;
    new_node->size_exec = executable_count;
    return new_node;
}

t_constructor *fill_constructor_manually(t_shell *shell)
{
    int size;
    printf("¿Cuántos nodos desea crear? ");
    if (scanf("%d", &size) != 1 || size < 0)
    {
        fprintf(stderr, "Error: Número de nodos no válido.\n");
        return NULL;
    }
    getchar();
    shell->node_size = size;
    t_constructor *head = NULL;
    t_constructor *tail = NULL;

    for (int i = 0; i < size; i++)
    {
        t_constructor *new_node = create_single_constructor_node(i);
        if (!new_node)
        {
            while (head)
            {
                t_constructor *temp = head;
                head = head->next;
                clean_constructor(temp);
            }
            return NULL;
        }

        if (!head)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            tail->next = new_node;
            tail = new_node;
        }
    }

    return head;
}