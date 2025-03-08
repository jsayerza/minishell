#include "../minishell.h"

#include "../minishell.h"

t_constructor *create_single_constructor_node(int node_number)
{
	t_constructor *new_node = (t_constructor *)malloc(sizeof(t_constructor));
	if (!new_node)
	{
		perror("malloc");
		return NULL;
	}

	new_node->paths = NULL;
	new_node->executable = NULL;
	new_node->size_exec = 0;
	new_node->builtin = BUILTIN_NONE;
	new_node->type = TOKEN_COMMAND;
	new_node->error = NO_ERROR;
	new_node->next = NULL;
	printf("\n----- Nodo %d -----\n", node_number);
	printf("Ingrese el tipo de token (0-8):\n");
	printf("0: TOKEN_WORD\n1: TOKEN_COMMAND\n2: TOKEN_BUILTIN\n3: TOKEN_PIPE\n");
	printf("4: TOKEN_REDIRECT_IN\n5: TOKEN_REDIRECT_OUT\n6: TOKEN_APPEND\n");
	printf("7: TOKEN_HEREDOC\n8: TOKEN_EOF\n");
	int token_type;
	if (scanf("%d", &token_type) != 1 || token_type < 0 || token_type > 8)
	{
		fprintf(stderr, "Error: Tipo de token no válido.\n");
		free(new_node);
		return NULL;
	}
	getchar();
	new_node->type = (e_token_type)token_type;
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
		new_node->builtin = (e_builtin)builtin;
	}
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
