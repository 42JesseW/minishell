/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bi_export.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/29 16:07:41 by jessevander...#+#    #+#                 */
/*   Updated: 2021/12/29 16:07:41 by jessevander...########   odam.nl         */
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
			if (!ft_isalnum(pair->key[idx]))
			{
				if (!(pair->key[idx] == '+' && !pair->key[idx + 1]))
					return (false);
			}
		}
		idx++;
	}
	return (true);
}

void	invalid_key_msg(char *key, int *exit_code)
{
	if (*exit_code == EXIT_SUCCESS)
		*exit_code = EXIT_FAILURE;
	ft_dprintf(STDERR_FILENO, "%s: export: `%s': not a valid identifier\n",
		SHELL_NAME, key);
}

int	export(t_exe *exe, t_pair *pair)
{
	bool	append;
	char	*key;
	int		exit_code;

	append = false;
	if (ft_strchr(pair->key, '+'))
	{
		append = true;
		key = ft_strdup(pair->key);
		if (!key)
			return (SYS_ERROR);
		key[ft_strlen(key) - 1] = 0;
	}
	else
		key = pair->key;
	exit_code = environ_update(exe->environ, key, pair->val, append);
	if (append)
		free(key);
	return (exit_code);
}

int	export_write_stdout(t_list *environ)
{
	(void)environ;
	printf("TEST\n");
	return (EXIT_SUCCESS);
}

int	builtin_export(char **cmd, t_exe *exe)
{
	t_pair	*pair;
	int		exit_code;
	int		idx;

	if (!cmd || !cmd[0] || ft_strcmp(cmd[0], "export") != 0)
		return (EXIT_FAILURE);
	if (!cmd[1])
		return (export_write_stdout(*exe->environ));
	idx = 1;
	exit_code = EXIT_SUCCESS;
	while (cmd[idx])
	{
		pair = pair_new_val(cmd[idx]);
		if (!pair)
			return (SYS_ERROR);
		if (is_valid_key(pair))
		{
			if (export(exe, pair) == SYS_ERROR)
				return (SYS_ERROR);
		}
		else
			invalid_key_msg(cmd[idx], &exit_code);
		pair_del(pair);
		idx++;
	}
	return (exit_code);
}
