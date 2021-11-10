/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_get.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/06 16:05:22 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/06 16:05:22 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_env	*env_lst_get(t_env **root, char *key)
{
	t_env	*traverse_env;

	if (!root || !(*root))
		return (NULL);
	traverse_env = *root;
	while (traverse_env)
	{
		if (strcmp(traverse_env->pair->key, key) == 0)
			return (traverse_env);
		traverse_env = traverse_env->next;
	}
	return (NULL);
}
