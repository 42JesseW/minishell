/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_lst_put.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/06 16:10:38 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/06 16:10:38 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** DESCRIPTION
**	- Check if matched node can stay:
**		1. Existing node has a value and new node does not
**		2. Both nodes do not have a value
**	- Check if the new node matched the root node. In this
**	  case the pointer to the root should be changed and
**	  no previous node exists so this should be skipped.
*/

static t_env	*update_env(t_env **root, t_env *prev, t_env *match, t_env *env)
{
	if ((!match->pair->val && !env->pair->val)
		|| (match->pair->val && !env->pair->val))
	{
		env_del(&env);
		return (match);
	}
	env->next = match->next;
	if (*root == match)
		*root = env;
	else
		prev->next = env;
	env_del(&match);
	return (env);
}

/*
** DESCRIPTION
**	- Add a new environment variable to the list.
**	  If the key does not exist then the new node
**	  is added to the back of the list. If the key
**	  of the new node already exists in the list,
**	  then the existing node is updated and the new
**	  node is freed.
**
** RETURN
**	- A pointer to an env structure that corresponds
**	  with either a newly added node or an existing
**	  node that corresponded with {env.pair.key}.
*/

t_env	*env_lst_put(t_env **root, t_env *env)	// TODO testcase
{
	t_env	*traverse_env;
	t_env	*prev;

	if (!root)
		return (NULL);
	if (!(*root))
		*root = env;
	else
	{
		prev = NULL;
		traverse_env = *root;
		while (traverse_env)
		{
			if (strcmp(traverse_env->pair->key, env->pair->key) == 0)	// TODO use ft_strcmp
				return (update_env(root, prev, traverse_env, env));
			prev = traverse_env;
			traverse_env = traverse_env->next;
		}
		prev->next = env;
	}
	return (env);
}
