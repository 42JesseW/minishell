/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstinsert.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: null <null@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/23 07:24:43 by null          #+#    #+#                 */
/*   Updated: 2021/11/23 07:24:43 by null          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static void	link_node(t_list *prev, t_list *next, t_list *node)
{
	node->next = next;
	prev->next = node;
}

/* insert node in list starting from root at position idx */
void	ft_lstinsert(t_list **root, t_list *node, int idx)
{
	t_list	*traverse;
	t_list	*prev;
	int		size;
	int		i;

	size = ft_lstsize(*root);
	if ((size == 0 && idx > 0) || (idx > size))
		return ;
	if (idx == 0)
		ft_lstadd_front(root, node);
	else if (idx == size)
		ft_lstadd_back(root, node);
	else
	{
		i = 1;
		prev = *root;
		traverse = prev->next;
		while (i < idx)
		{
			prev = traverse;
			traverse = traverse->next;
			i++;
		}
		link_node(prev, traverse, node);
	}
}
