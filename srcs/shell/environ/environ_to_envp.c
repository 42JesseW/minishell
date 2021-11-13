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

/*
** environ_to_envp() converts a list of
** pairs, the environment variables, back
** to an array of strings.
*/

char	**environ_to_envp(t_list *root)
{
	t_list 	*traverse_lst;
	char	**envp;
	char	*kv;
	int		idx;

	idx = 0;
    traverse_lst = root;
	envp = (char **)malloc(sizeof(char *) * (ft_lstsize(root) + 1));
	if (!envp)
		return (NULL);
	while (traverse_lst)
    {
		kv = pair_join((t_pair *)traverse_lst->content);
		if (!kv)
		{
			free_array_part(envp, idx);
			return (NULL);
		}
        traverse_lst = traverse_lst->next;
		envp[idx] = kv;
		idx++;
	}
	envp[idx] = NULL;
	return (envp);
}
