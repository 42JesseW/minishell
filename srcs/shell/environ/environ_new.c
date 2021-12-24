/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   environ_new.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/24 16:26:33 by jessevander...#+#    #+#                 */
/*   Updated: 2021/12/24 16:26:33 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/* create a new environment variable as a t_list type */
t_list *environ_new(char *key, const char *val)
{
	t_list	*node;
	t_pair	*pair;

	pair = pair_new_kv(key, val);
	if (!pair)
		return (NULL);
	node = ft_lstnew(pair);
	if (!node)
	{
		pair_del(pair);
		return (NULL);
	}
	return (node);
}
