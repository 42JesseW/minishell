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

int	check_special_builtin(t_builtin *builtin, char **cmd)
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
	*exe->exit_code = exit_code;
	if (exit_code == SYS_ERROR)
		return (SYS_ERROR);
	else
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
