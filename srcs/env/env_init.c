/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_init.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/05 16:37:17 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/05 16:37:17 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** NOTES
**	- "key=" is a valid string to pass to
**	  this function and will set pair.val
**	  to NULL.
*/

t_pair	*env_init_create_pair(const char *env)
{
	t_pair	*pair;
	char	**kv;

	kv = ft_strsplit(env, '=');
	if (!kv)
		return (NULL);
	pair = pair_new_val(kv[0], kv[1]);
	if (!pair)
	{
		ft_strarrfree(&kv);
		return (NULL);
	}
	return (pair);
}

/*
** DESCRIPTION
**	- Convert incoming char* environment variables to
**	  t_pair structures and add them to a linked list
**	  with t_env nodes.
**
** NOTES
**	- *envp[] only contains strings with format key=val
**	  where a '=' is always present.
*/

int	env_init(t_env **root, const char *envp[])
{
	t_pair	*pair;
	t_env	*env;
	int		idx;

	idx = 0;
	while (envp[idx])
	{
		pair = env_init_create_pair(envp[idx]);
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
		env_lst_add_back(root, env);
		idx++;
	}
	return (EXIT_SUCCESS);
}
