/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_lst_add_back.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/05 17:34:42 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/05 17:34:42 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	env_lst_add_back(t_env **root, t_env *env)
{
	t_env	*traverse_env;

	if (!root)
		return ;
	if (!(*root))
		*root = env;
	else
	{
		traverse_env = *root;
		while (traverse_env)
		{
			if (!traverse_env->next)
			{
				traverse_env->next = env;
				break ;
			}
			traverse_env = traverse_env->next;
		}
	}
}
