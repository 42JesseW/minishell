/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_del.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/05 16:37:06 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/05 16:37:06 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
//test
void	env_del(t_env **env)
{
	t_env	*p;

	p = *env;
	if (p)
	{
		pair_del(&p->pair);
		free(p);
	}
	*env = NULL;
}
