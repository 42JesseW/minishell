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

void	invalid_key_msg(char *key, int *exit_code)
{
	if (*exit_code == EXIT_SUCCESS)
		*exit_code = EXIT_FAILURE;
	ft_dprintf(STDERR_FILENO,
		"%s: export: `%s': not a valid identifier\n",
		SHELL_NAME,
		key);
}

bool	is_valid_key(char *key)
{
	int	idx;

	idx = 0;
	while (key[idx])
	{
		if (idx == 0 && !(ft_isalpha(key[idx]) || key[idx] == '_'))
			return (false);
		else
		{
			if (!(ft_isalnum(key[idx]) || key[idx] == '_'))
			{
				if (!(key[idx] == '+' && !key[idx + 1]))
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
