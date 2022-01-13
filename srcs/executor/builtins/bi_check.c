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

static void	backup_fds(int *fds)
{
	fds[STDIN_FILENO] = dup(STDIN_FILENO);
	fds[STDOUT_FILENO] = dup(STDOUT_FILENO);
}

static int	reset_external_cmd_env(t_exe *exe, t_node *node)
{
	int		idx;

	if (!node->cmd || !node->cmd[0])
		return (SUCCESS);
	if (exe->amount_cmds > 1 || node->redir)
	{
		if (environ_update(exe->environ, "_", "", false) == SYS_ERROR)
			return (SYS_ERROR);
	}
	else
	{
		idx = 0;
		while (node->cmd[idx + 1])
			idx++;
		if (environ_update(exe->environ, "_", node->cmd[idx], false)
			== SYS_ERROR)
			return (SYS_ERROR);
	}
	return (SUCCESS);
}

int	builtin_check(int idx, t_node *node, t_exe *exe)
{
	t_list		*bi_node;
	t_builtin	*builtin;
	int			fds[2];

	if (reset_external_cmd_env(exe, node) == SYS_ERROR)
		return (SYS_ERROR);
	backup_fds(fds);
	bi_node = exe->builtins;
	while (bi_node)
	{
		builtin = bi_node->content;
		if (node->cmd && ft_strcmp(node->cmd[0], builtin->name) == 0)
		{
			if (execute_builtin(idx, builtin, node, exe) == SYS_ERROR)
				return (SYS_ERROR);
			if (restore_fds(fds[STDOUT_FILENO], fds[STDIN_FILENO]) == SYS_ERROR)
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
