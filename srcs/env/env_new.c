/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_new.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/05 17:30:21 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/05 17:30:21 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_env	*env_new_def(void)
{
	t_env	*env;

	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->pair = NULL;
	env->next = NULL;
	return (env);
}

t_env	*env_new_cpy(t_env *cpy)
{
	t_env	*env;

	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->pair = pair_new_cpy(cpy->pair);
	env->next = cpy->next;
	return (env);
}

t_env	*env_new_val(t_pair *pair)
{
	t_env	*env;

	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->pair = pair_new_cpy(pair);
	env->next = NULL;
	return (env);
}
