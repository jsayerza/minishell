/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:30:36 by jsayerza          #+#    #+#             */
/*   Updated: 2024/09/19 18:02:08 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list    *ft_lstnew(void *content)
{
     t_list    *newnode;

     if (!content)
          return (NULL);
     newnode =(t_list *)malloc(sizeof(t_list));
     if (!newnode)
          return (NULL);
     newnode->content = content;
     newnode->next = NULL;
     return (newnode);
}