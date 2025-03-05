#include <stdio.h>
#include <unistd.h>

int	main(char **env)
{
	const char *arv[] = {"echo", NULL};

	execve("/bin/echo", *arv, env);
	perror("execl failed");
	return 1;
}
