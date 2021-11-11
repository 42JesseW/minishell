/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_from_envp.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/10 08:34:23 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/10 08:34:23 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** DESCRIPTION
**	- Convert incoming char* environment variables to
**	  t_pair structures and add them to a linked list
**	  with t_env nodes.
**
** NOTES
**	- a char* environment variable can be key=''
**	  where the key is "key" and the value is ""
**	- a char* environment variable can be key=a=b=c
**	  where the key is "key" and the value is "a=b=c"
*/

int	env_from_envp(t_env **root, const char **envp)
{
	t_pair	*pair;
	t_env	*env;
	int		idx;

	idx = 0;
	while (envp[idx])
	{
		pair = pair_new_val(envp[idx]);
		if (!pair)
		{
			env_lst_del(root);
			return (SYS_ERROR);
		}
		env = env_new_val(pair);
		if (!env)
		{
			env_lst_del(root);
			return (SYS_ERROR);
		}
		env_lst_put(root, env);
		idx++;
	}
	return (EXIT_SUCCESS);
}
