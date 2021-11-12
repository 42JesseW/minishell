/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_to_envp.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/08 07:14:24 by null          #+#    #+#                 */
/*   Updated: 2021/11/08 07:14:24 by null          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	free_array_part(char **envp, int idx)
{
	int	reverse_idx;

	reverse_idx = idx;
	while (reverse_idx >= 0)
	{
		free((void *)envp[reverse_idx]);
		reverse_idx--;
	}
	free(envp);
}

char	**env_to_envp(t_env *root)
{
	t_env	*traverse_env;
	char	**envp;
	char	*kv;
	int		idx;

	idx = 0;
	traverse_env = root;
	envp = (char **)malloc(sizeof(char *) * (env_lst_len(root) + 1));
	if (!envp)
		return (NULL);
	while (traverse_env)
	{
		kv = pair_join(traverse_env->pair);
		if (!kv)
		{
			free_array_part(envp, idx);
			return (NULL);
		}
		traverse_env = traverse_env->next;
		envp[idx] = kv;
		idx++;
	}
	envp[idx] = NULL;
	return (envp);
}
