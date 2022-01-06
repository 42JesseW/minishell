/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstadd_front.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/18 19:52:28 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/10/17 14:21:10 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *node)
{
	if (node == NULL)
		return ;
	else if (*lst == NULL)
		*lst = node;
	else
	{
		node->next = *lst;
		*lst = node;
	}
	return ;
}
