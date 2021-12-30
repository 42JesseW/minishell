/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   environ_update.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/24 16:27:06 by jessevander...#+#    #+#                 */
/*   Updated: 2021/12/24 16:27:06 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	create_new(t_list **environ, char *key, const char *val)
{
	t_list	*node;

	node = environ_new(key, val);
	if (!node)
		return (SYS_ERROR);
	ft_lstadd_front(environ, node);
	return (SUCCESS);
}

/*
** environ_update() will try to find a matching
** environment variable using a key and replaces
** the value with a new val if found. If it can
** not find a matching key, It will create a new
** environment variable and add it to the list.
*/
int	environ_update(t_list **environ, char *key, const char *val, bool append)
{
	t_list	*node;
	t_pair	*pair;
	char	*new_val;

	node = *environ;
	while (node)
	{
		pair = node->content;
		if (ft_strcmp(pair->key, key) == 0)
		{
			if (!val && pair->val)
				return (SUCCESS);
			if (append)
				new_val = ft_strjoin(pair->val, val);
			else
				new_val = ft_strdup(val);
			if (!new_val)
				return (SYS_ERROR);
			free(pair->val);
			pair->val = new_val;
			return (SUCCESS);
		}
		node = node->next;
	}
	return (create_new(environ, key, val));
}
