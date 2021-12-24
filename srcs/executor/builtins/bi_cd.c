/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bi_cd.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/24 13:20:15 by jessevander...#+#    #+#                 */
/*   Updated: 2021/12/24 13:20:15 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO increment SHLVL on startup of program
// TODO remove OLDPWD on startup of program

t_list *environ_new(char *key, const char *val)
{
	t_list	*node;
	t_pair	*pair;

	pair = pair_new_kv(key, val);
	if (!pair)
		return (NULL);
	node = ft_lstnew(pair);
	if (!node)
	{
		pair_del(pair);
		return (NULL);
	}
	return (node);
}

int	environ_update(t_list **environ, char *key, const char *val)	// TODO testcase
{
	t_list	*node;
	t_pair	*pair;

	node = *environ;
	while (node)
	{
		pair = node->content;
		if (ft_strcmp(pair->key, key) == 0)
		{
			free(pair->val);
			pair->val = ft_strdup(val);
			if (!pair->val)
				return (SYS_ERROR);
			return (SUCCESS);
		}
		node = node->next;
	}
	node = environ_new(key, val);
	if (!node)
		return (SYS_ERROR);
	ft_lstadd_front(environ, node);
	return (SUCCESS);
}

int	reset_pwd_env(t_list **environ)
{
	const char	*pwd_val;
	char		cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
		return (SYS_ERROR);
	pwd_val = environ_get(*environ, "PWD");
	if (pwd_val)
	{
		if (environ_update(environ, "OLDPWD", pwd_val) == SYS_ERROR)
			return (SYS_ERROR);
	}
	return (environ_update(environ, "PWD", cwd));
}

int	cd_env(t_list **environ, char *env)
{
	const char	*dir;
	int 		ret;

	dir = environ_get(*environ, env);
	if (!dir)
	{
		ft_dprintf(STDERR_FILENO, "%s: cd: %s not set\n", SHELL_NAME, env);
		return (EXIT_FAILURE);
	}
	ret = chdir(dir);
	if (ret == SYS_ERROR)
	{
		ft_dprintf(STDERR_FILENO, "%s: cd: %s: %s\n", SHELL_NAME, dir, strerror(errno));
		return (EXIT_FAILURE);
	}
	if (ft_strcmp(env, "OLDPWD") == 0)
		ft_printf("%s\n", dir);
	reset_pwd_env(environ);
	return (EXIT_SUCCESS);
}

/*
** builtin_cd() uses chdir to change the current
** working directory of the program.
***
** It also manages the environment variables OLDPWD
** and PWD. The rules are as follows:
**	1. OLDPWD does not exist on startup.
**	2. OLDPWD is created only if PWD exists
**	3. OLDPWD is set to PWD only chdir is successful
**	4. PWD is created using getcwd if it does not exist
**	4. PWD is set to new directory only if chdir is successful
**
*/
int builtin_cd(char **cmd, t_exe *exe)
{
	int	ret;

	if (!cmd || !cmd[0] || ft_strcmp(cmd[0], "cd") != 0)
		return (EXIT_FAILURE);
	/* if only cd go to HOME */
	if (!cmd[1])
		return (cd_env(exe->environ, "HOME"));
	/* if `cd -` go to previous directory */
	if (ft_strcmp(cmd[1], "-") == 0)
		return (cd_env(exe->environ, "OLDPWD"));
	ret = chdir(cmd[1]);
	if (ret == SYS_ERROR)
	{
		ft_dprintf(STDERR_FILENO, "%s: cd: %s: %s\n", SHELL_NAME, cmd[1], strerror(errno));
		return (EXIT_FAILURE);
	}
	reset_pwd_env(exe->environ);
	return (EXIT_SUCCESS);
}
