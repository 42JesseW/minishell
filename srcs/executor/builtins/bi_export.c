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

static int	export(t_exe *exe, t_pair *pair)
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

static void	write_stdout(char *arg, char **split)
{
	int	idx;

	if (ft_strchr(arg, '=') && !split[1])
		ft_printf("=\"\"");
	if (split[1])
	{
		idx = 2;
		ft_printf("=\"%s", split[1]);
		while (split[idx])
		{
			ft_printf("=%s", split[idx]);
			idx++;
		}
		ft_printf("\"");
	}
}

static int	export_write_stdout(t_list *environ)
{
	char	**sorted;
	char	**split;
	char	**envp;
	int		idx;

	envp = environ_to_envp(environ);
	sorted = envp_lexical_sort(envp);
	if (!sorted)
		return (EXIT_FAILURE);
	idx = 0;
	while (sorted[idx])
	{
		split = ft_strsplit(sorted[idx], '=');
		if (!split)
			return (EXIT_FAILURE);
		ft_printf("declare -x %s", split[0]);
		write_stdout(sorted[idx], split);
		ft_printf("\n");
		ft_strarrfree(&split);
		idx++;
	}
	ft_strarrfree(&envp);
	return (EXIT_SUCCESS);
}

int	builtin_export(char **cmd, t_exe *exe)
{
	t_pair	*pair;
	int		exit_code;
	int		idx;

	if (!cmd[1])
		return (export_write_stdout(*exe->environ));
	idx = 1;
	exit_code = EXIT_SUCCESS;
	while (cmd[idx])
	{
		pair = pair_new_val(cmd[idx]);
		if (!pair)
			return (SYS_ERROR);
		if (ft_strlen(pair->key) > 0 && is_valid_key(pair->key))
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
