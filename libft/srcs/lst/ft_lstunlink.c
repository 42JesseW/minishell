/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstunlink.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: null <null@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/23 07:37:46 by null          #+#    #+#                 */
/*   Updated: 2021/11/23 07:37:46 by null          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

/* unlink node from list starting at root returning node */
t_list	*ft_lstunlink(t_list **root, t_list *node)
{
	t_list	*traverse;
	t_list	*prev;

	if (!root || !(*root))
		return (NULL);
	prev = NULL;
	traverse = *root;
	while (traverse)
	{
		if (traverse == node)
		{
			if (!prev)
				*root = traverse->next;
			else
				prev->next = traverse->next;
			traverse->next = NULL;
			return (traverse);
		}
		prev = traverse;
		traverse = traverse->next;
	}
	return (NULL);
}
