/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_builtin.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/13 13:15:35 by jessevander...#+#    #+#                 */
/*   Updated: 2022/01/13 13:15:35 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	check_special_builtin(t_builtin *builtin, char **cmd)
{
	if (ft_strcmp(builtin->name, "cd") == 0 || ft_strcmp(builtin->name
			, "exit") == 0 || ft_strcmp(builtin->name, "unset") == 0)
		return (SUCCESS);
	if (ft_strcmp(builtin->name, "env") == 0 && ft_strarrlen(cmd) > 1)
	{
		printf("env: %s: No such file or directory\n\r", cmd[1]);
		return (SUCCESS);
	}
	else
		return (0);
}

int	execute_builtin(int idx, t_builtin *builtin, t_node *node, t_exe *exe)
{
	int	exit_code;

	if (exe->amount_cmds > 1)
	{
		if (check_special_builtin(builtin, node->cmd) == SUCCESS)
			return (SUCCESS);
		if (dup_pipes(idx, 1, exe) == SYS_ERROR)
			return (SYS_ERROR);
	}
	if (ft_lstsize(node->redir) > 0)
	{
		if (dup_redirect(node) == SYS_ERROR)
			return (SUCCESS);
	}
	exit_code = (*builtin->function)(node->cmd, exe);
	if (ft_strcmp(builtin->name, "exit") != 0)
		*exe->exit_code = exit_code;
	if (exit_code == SYS_ERROR)
		return (SYS_ERROR);
	else
		return (SUCCESS);
}
