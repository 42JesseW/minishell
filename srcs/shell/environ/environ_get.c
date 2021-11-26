/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   environ_get.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/25 16:01:15 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/25 16:01:15 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** environ_get() returns a pointer to the string
** held by the t_pair structure if key matches
** pair.key of an existing environment variable.
*/
const char	*environ_get(t_list *environ, const char *key)
{
	t_list	*traverse;
	t_pair	*pair;

	if (!environ || !key)
		return (NULL);
	traverse = environ;
	while (traverse)
	{
		pair = (t_pair *)traverse->content;
		if (ft_strncmp(pair->key, key, ft_strlen(key)) == 0)
			return (pair->val);
		traverse = traverse->next;
	}
	return (NULL);
}
