/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bi_check.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/17 10:16:40 by annaheister   #+#    #+#                 */
/*   Updated: 2021/12/17 10:16:40 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	restore_fds(int old_stdout, int old_stdin)
{
	if (dup2(old_stdout, STDOUT_FILENO) == -1
		|| dup2(old_stdin, STDIN_FILENO) == -1)
		return (SYS_ERROR);
	if (close(old_stdout) == -1 || close(old_stdin) == -1)
		return (SYS_ERROR);
	return (SUCCESS);
}

int	check_special_builtin(t_builtin *builtin)
{
	if (ft_strcmp(builtin->name, "cd") == 0 || ft_strcmp(builtin->name
			, "exit") == 0 || ft_strcmp(builtin->name, "unset") == 0)
		return (SUCCESS);
	else
		return (0);
}

int	execute_builtin(int idx, t_builtin *builtin, t_node *node, t_exe *exe)
{
	if (exe->amount_cmds > 1)
	{
		if (check_special_builtin(builtin) == SUCCESS)
			return (SUCCESS);
		if (dup_pipes(idx, 1, exe) == SYS_ERROR)
			return (SYS_ERROR);
	}
	if (ft_lstsize(node->redir) > 0)
	{
		if (dup_redirect(node) == SYS_ERROR)
			return (SYS_ERROR);
	}
	if ((*builtin->function)(node->cmd, exe) == SYS_ERROR)
		return (SYS_ERROR);
	else
		return (SUCCESS);
}

int	builtin_check(int idx, t_node *node, t_exe *exe)
{
	t_list		*bi_node;
	t_builtin	*builtin;
	int			old_stdout;
	int			old_stdin;

	bi_node = exe->builtins;
	old_stdout = dup(STDOUT_FILENO);
	old_stdin = dup(STDIN_FILENO);
	while (bi_node)
	{
		builtin = bi_node->content;
		if (ft_strcmp(node->cmd[0], builtin->name) == 0)
		{
			if (execute_builtin(idx, builtin, node, exe) == SYS_ERROR)
				return (SYS_ERROR);
			if (restore_fds(old_stdout, old_stdin) == SYS_ERROR)
				return (SYS_ERROR);
			else
				return (SUCCESS);
		}
		bi_node = bi_node->next;
	}
	if (fork_process(idx, exe, node) == SYS_ERROR)
		return (SYS_ERROR);
	return (SUCCESS);
}
