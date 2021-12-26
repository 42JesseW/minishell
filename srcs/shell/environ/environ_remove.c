/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   environ_remove.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/24 16:30:01 by jessevander...#+#    #+#                 */
/*   Updated: 2021/12/24 16:30:01 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/* remove an environment variable from the list matching on key */
void	environ_remove(t_list **environ, char *key)
{
	t_list	*node;
	t_pair	*pair;

	node = *environ;
	while (node)
	{
		pair = (t_pair *)node->content;
		if (ft_strcmp(pair->key, key) == 0)
		{
			ft_lstunlink(environ, node);
			ft_lstdelone(node, pair_del);
			break ;
		}
		node = node->next;
	}
}
