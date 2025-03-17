#include "../minishell.h"

void pwd()
{
	char *pwd;

	pwd = getcwd( NULL , 0);
	if (pwd)
		printf("%s\n", pwd);
	else
		printf("minishell: pwd: PWD not set\n");
}
