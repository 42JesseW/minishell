/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bi_export_utils.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/30 14:29:38 by jessevander...#+#    #+#                 */
/*   Updated: 2021/12/30 14:29:38 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	is_valid_key(t_pair *pair)
{
	int	idx;

	idx = 0;
	while (pair->key[idx])
	{
		if (idx == 0 && !(ft_isalpha(pair->key[idx]) || pair->key[idx] == '_'))
			return (false);
		else
		{
			if (!(ft_isalnum(pair->key[idx]) || pair->key[idx] == '_'))
			{
				if (!(pair->key[idx] == '+' && !pair->key[idx + 1]))
					return (false);
			}
		}
		idx++;
	}
	return (true);
}

static int	envp_lexical_swap(char **envp, int idx, int i)
{
	char	*dup[2];

	dup[0] = ft_strdup(envp[idx]);
	dup[1] = ft_strdup(envp[i]);
	if (!dup[0] || !dup[1])
	{
		free(dup[0]);
		free(dup[1]);
		return (-1);
	}
	free(envp[idx]);
	free(envp[i]);
	envp[idx] = dup[1];
	envp[i] = dup[0];
	return (1);
}

char	**envp_lexical_sort(char **envp)
{
	int	idx;
	int	i;

	if (!envp)
		return (NULL);
	idx = 0;
	while (envp[idx])
	{
		i = idx + 1;
		while (envp[i])
		{
			if (ft_strcmp(envp[idx], envp[i]) > 0)
			{
				if (envp_lexical_swap(envp, idx, i) < 0)
					return (NULL);
			}
			i++;
		}
		idx++;
	}
	return (envp);
}
