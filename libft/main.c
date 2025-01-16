/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:22:09 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/18 13:05:00 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

void	del(void *content)
{
	free(content);
}

void	print_node_list(t_list *nodes)
{
	while (nodes)
	{
		printf("Content:%s\n", (char *)nodes->content);
		nodes = nodes->next;
	}
}

void	test_ft_lst(void)
{
	t_list	*nodes, *segonnode, *tercernode;

	char	*str = "Node 1. LA Cosa es GRAn.";
	nodes = ft_lstnew((void *)str);
	if (nodes)
		printf("Node1: %s\n", (char *)nodes->content);
	else
		printf("KKlavaK\n");

	str = "Node 2. Una altra cosa nodosa.";
	segonnode = ft_lstnew((void *)str);
	if (segonnode)
		printf("Node2:%s\n", (char *)segonnode->content);
	else
		printf("KKlavaK\n");
	print_node_list(nodes);
	ft_lstadd_front(&nodes, segonnode);
	print_node_list(nodes);

	printf("Num Nodes: %d\n", ft_lstsize(nodes));

	printf("Last Node: %s\n", (char *)ft_lstlast(nodes)->content);

	tercernode = ft_lstnew("Node 3. Tachaaan!");
	ft_lstadd_back(&nodes, (t_list *)tercernode);
//	ft_lstadd_back(&nodes, (t_list *)ft_lstnew("Node 3. Tachaaan!"));
	printf("Num Nodes: %d\n", ft_lstsize(nodes));
	printf("Last Node: %s\n", (char *)ft_lstlast(nodes)->content);

	//ft_lstdelone(tercernode, del);
	printf("Num Nodes:%d\n", ft_lstsize(nodes));
	printf("Last Node:%s\n", (char *)ft_lstlast(nodes)->content);

	free(nodes->next);
	free(nodes);
}

void	test_ft_put_fd(void)
{
	char	str[] = "LA Cosa es GRAn.";

	ft_putchar_fd('K', 1);
	ft_putstr_fd(str, 1);
	ft_putendl_fd(str, 1);
	
	ft_putnbr_fd(0, 1);
	ft_putchar_fd('\n', 1);
	ft_putnbr_fd(1234, 1);
	ft_putchar_fd('\n', 1);
	ft_putnbr_fd(-1234, 1);
	ft_putchar_fd('\n', 1);
	ft_putnbr_fd(2147483647, 1);
	ft_putchar_fd('\n', 1);
	ft_putnbr_fd(-2147483648, 1);
	ft_putchar_fd('\n', 1);
}

void my_toupper_modi(unsigned int i, char *ptr)
{
	//printf("  ptr:%p  *ptr:%c", (void *)ptr, *ptr);
	i = i + 1;
    if (*ptr >= 'a' && *ptr <= 'z')
	{
		//printf("  --> *ptr-32:%c", *ptr-32);
        *ptr = *ptr - 32;
	}
}

void	test_ft_striteri(void)
{
	char	str[] = "LA Cosa es GRAn";

	ft_striteri(str , my_toupper_modi);
}

char my_toupper(unsigned int i, char c)
{
    i = i + 1;
    if (c >= 'a' && c <= 'z')
        return (c - 32);
    else
        return (c);
}

void	test_ft_strmapi(void)
{
    char *s = "heLLo World";
    char *res = ft_strmapi(s, my_toupper);

    if (res)
    {
        printf("Resultado: %s\n", res);
        free(res);
    }

}

void	test_ft_itoa(void)
{
	printf("res:%s\n", ft_itoa(1234));
	printf("res:%s\n", ft_itoa(-1234));
	printf("res:%s\n", ft_itoa(2147483647));
	printf("res:%s\n", ft_itoa(-2147483648));
	printf("res:%s\n", ft_itoa(0));
}

void	test_ft_split(void)
{
	// printf("res:%p\n", ft_split("xxx1x22x333xxxx4444x55555xxx", 'x'));
	// printf("res:%p\n", ft_split("1x22x333xxxx4444x55555xxx", 'x'));
	// printf("res:%p\n", ft_split("xxx1x22x333xxxx4444x55555", 'x'));

    char **res = ft_split("  Hola   caracola como mola!   ", ' ');

    for (int i = 0; res[i] != 0; i++)
    {
        printf("Word %d: %s\n", i, res[i]);
        free(res[i]);
    }
    free(res);

}

void	test_ft_strtrim(void)
{
	printf("res:%s\n", ft_strtrim("CCC1234567890XXX", "XCVB"));
}

void	test_ft_strjoin(void)
{
	printf("res:%s\n", ft_strjoin("1234567890", "abcde"));
	printf("res:%s\n", ft_strjoin("1234567890", ""));
	printf("res:%s\n", ft_strjoin("", "abcde"));
	printf("res:%s\n", ft_strjoin("", ""));
}

void	test_ft_substr(void)
{
	printf("res:%s\n", ft_substr("123456789o", 1, 4));
	printf("res:%s\n", ft_substr("123456789o", 4, 1));
	printf("res:%s\n", ft_substr("123456789o", 8, 4));
	printf("res:%s\n", ft_substr("123456789o", 10, 4));
	printf("res:%s\n", ft_substr("123456789o", 14, 4));
}

void	test_ft_strdup(void)
{
	char *s;
	
	s = "1234567890";
	printf("res:%s\n", ft_strdup(s));
	printf("ofi:%s\n", strdup(s));
}

void	test_ft_calloc(void)
{
	size_t tipo_dato;
	size_t grandezza_dato;

	tipo_dato = 100;//integer data type quantity of bytes that defines an inti
	grandezza_dato = 50;//Quantity of data
	printf("ofi>%p\n", calloc(tipo_dato, grandezza_dato));
	printf("res>%p\n", ft_calloc(tipo_dato, grandezza_dato));
}

void	test_ft_atoi(void)
{
	printf("res:%d\n", ft_atoi("1234a5"));
	printf("res:%d\n", ft_atoi("-1234"));
	printf("res:%d\n", ft_atoi("2147483647"));
	printf("res:%d\n", ft_atoi("-2147483648"));
	printf("res:%d\n", ft_atoi("0"));
}

void	test_ft_memcmp(void)
{
	char	a1[]="1234567890";
	char	a2[]="123457890";
	printf("res:%d\n", ft_memcmp(a1,a2,6));
}

void	test_ft_memchr(void)
{
	char	array[]="1234567890";
	int	c;
	size_t	numberof;
	char	*ptr;

	c = '4';
	numberof = 7;	
	ptr = ft_memchr(array, c, numberof);
	printf("array_i:%s\n", ptr);
}

void	test_ft_memmove(void)
{

	char array[]="1234567890";
	size_t numberof;

	numberof = 3;
	printf("array_i:%s\n",array);
	ft_memmove(array+3, array+2, numberof);
	printf("array_f:%s\n",array);
}


void	test_ft_memcpy(void)
{

	char array[20]="hola estrella";
	char array2[20]="maravella";
	size_t numberof;

	numberof = 5;
	printf("array_i:%s\n",array);
	ft_memcpy(array, array2, numberof);
	printf("array_f:%s\n",array);
}

void	test_ft_memset(void)
{

	char array[20]="hola estrella";
	int array_int[7]= {-1,5,200,13,1,11,10};
	size_t numberof;
	int i;

	numberof = 5;
	printf("array_i:%s\n",array);
	ft_memset(array, 'a', numberof);
	printf("array_f:%s\n",array);
	ft_memset(array_int, 8, 4);	
	i = 0;
	while (i < 7)
	{
		printf("[%d]\n", array_int[i]);
		i++;
	}
}

void	test_ft_bzero(void)
{

	char array[20]="hola estrella";
	int array_int[7]= {-1,5,200,13,1,11,10};
	size_t numberof;
	int i;

	numberof = 5;
	printf("array_i:%s\n",array);
	ft_bzero(array, numberof);
	printf("array_f:%s\n",array);
	ft_bzero(array_int, 4);	
	i = 0;
	while (i < 7)
	{
		printf("[%d]\n", array_int[i]);
		i++;
	}
}

void	test_ft_tolower(void)
{
	printf("c:%c\n",ft_tolower('a'));
	printf("c:%c\n",ft_tolower('z'));
	printf("c:%c\n",ft_tolower('A'));
	printf("c:%c\n",ft_tolower('Z'));
	printf("c:%c\n",ft_tolower('\n'));
	printf("c:%c\n",ft_tolower('0'));
}

void	test_ft_toupper(void)
{
	printf("c:%c\n",ft_toupper('a'));
	printf("c:%c\n",ft_toupper('z'));
	printf("c:%c\n",ft_toupper('A'));
	printf("c:%c\n",ft_toupper('\n'));
	printf("c:%c\n",ft_toupper('0'));
}


void	test_ft_strlcat(void)
{
    char first[] = "12345";
    char last[] = "678901234567890";
    int r;
    int size = 21;
    char buffer[size];

    ft_strlcpy(buffer,first, size);
    r = ft_strlcat(buffer,last,size);
    puts(buffer);
    printf("Value returned: %d\n",r);
    if (r > size)
	{
        puts("String truncated");
	}
    else
	{
        puts("String was fully copied");
	}
}

void	test_ft_strlcpy(void)
{
    char string[] = "Hello there, Venus";
    char buffer[19];
    int r, size;

	size = 19;
    r = ft_strlcpy(buffer,string,size);
    printf("Copied '%s' into '%s', length %d\n", string, buffer, r);
	size = 10;
    r = ft_strlcpy(buffer,string,size);
    printf("Copied '%s' into '%s', length %d\n", string, buffer, r);
	size = 1;
    r = ft_strlcpy(buffer,string,size);
    printf("Copied '%s' into '%s', length %d\n", string, buffer, r);
	size = 0;
    r = ft_strlcpy(buffer,string,size);
    printf("Copied '%s' into '%s', length %d\n", string, buffer, r);
}

int	main(void)
{
//	test_ft_strlcpy();
//	test_ft_strlcat();
//	test_ft_toupper();
//	test_ft_tolower();
//	test_ft_bzero();
//	test_ft_memset();
//	test_ft_memcpy();
//	test_ft_memmove();
//	test_ft_memchr();
//	test_ft_memcmp();
//	test_ft_atoi();
//	test_ft_calloc();
//	test_ft_strdup();
//	test_ft_substr();
//	test_ft_strjoin();
//	test_ft_strtrim();
//	test_ft_split();
//	test_ft_itoa();
//	test_ft_strmapi();
//	test_ft_striteri();
//	test_ft_put_fd();
	test_ft_lst();
	return (0);
}
