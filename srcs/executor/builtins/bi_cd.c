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

int	reset_pwd(t_list *environ)
{
	char	buff[PATH_MAX];
	t_list	*node;
	t_pair	*pair;

	if (!getcwd(buff, PATH_MAX))
		return (SYS_ERROR);
	pair = pair_new_kv("PWD", buff);
	if (!pair)
		return (SYS_ERROR);
	node = ft_lstnew(pair);
	if (!node)
	{
		pair_del(pair);
		return (SYS_ERROR);
	}
}

int builtin_cd(char **cmd, t_exe *exe)
{
	if (!cmd || !cmd[0] || ft_strcmp(cmd[0], "cd") != 0)
		return (EXIT_FAILURE);
	/* reset PWD if it does not exist */
	if (!environ_get(exe->environ, "PWD"))
	{

	}
}
