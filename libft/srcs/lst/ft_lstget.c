/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstget.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/25 15:58:39 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/25 15:58:39 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

int	ft_lstget_idx(t_list *root, t_list *node)
{
	t_list	*traverse;
	int		idx;

	idx = 0;
	traverse = root;
	while (traverse)
	{
		if (traverse == node)
			return (idx);
		traverse = traverse->next;
		idx++;
	}
	return (-1);
}

t_list	*ft_lstget_node(t_list *root, int idx)
{
	t_list	*traverse;

	traverse = root;
	while (traverse && idx > 0)
	{
		traverse = traverse->next;
		idx--;
	}
	return (traverse);
}
