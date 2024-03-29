/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstadd_back.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/18 22:33:05 by jevan-de      #+#    #+#                 */
/*   Updated: 2020/07/07 08:37:01 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *node)
{
	t_list	*cur;

	if (node == NULL)
		return ;
	else if (*lst == NULL)
		*lst = node;
	else
	{
		cur = *lst;
		while (cur)
		{
			if (cur->next == NULL)
			{
				cur->next = node;
				return ;
			}
			cur = cur->next;
		}
	}
}
