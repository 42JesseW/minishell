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

int	environ_from_envp(t_list **root, const char **envp)
{
	t_pair	*pair;
	t_list	*env;
	int		idx;

	idx = 0;
	while (envp[idx])
	{
		pair = pair_new_val(envp[idx]);
		if (!pair)
		{
			ft_lstclear(root, pair_del);
			return (SYS_ERROR);
		}
		env = ft_lstnew(pair);
		if (!env)
		{
			ft_lstclear(root, pair_del);
			return (SYS_ERROR);
		}
		ft_lstadd_back(root, env);
		idx++;
	}
	return (EXIT_SUCCESS);
}
