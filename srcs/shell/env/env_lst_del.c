/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_lst_del.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/05 17:28:15 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/05 17:28:15 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	env_lst_del(t_env **root)
{
	t_env	*env;
	t_env	*temp;

	if (!root)
		return ;
	env = *root;
	while (env)
	{
		temp = env;
		env = env->next;
		env_del(&temp);
	}
	*root = NULL;
}
