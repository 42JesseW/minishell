/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fork.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/02 19:18:58 by annaheister   #+#    #+#                 */
/*   Updated: 2021/12/02 19:18:58 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO Function descriptions toevoegen

/*
** DESCRIPTION
**	- INVULLEN
** JOBS
** 1. INVULLEN
*/

int	store_pids(pid_t pid, t_exe *exe)
{
	pid_t	*p_pid;
	t_list	*node;

	p_pid = &pid;
	node = ft_lstnew(p_pid);
	if (node == NULL)
	{
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Malloc",
			strerror(errno));
		return (SYS_ERROR);
	}
	ft_lstadd_back(&exe->pids, node);
	return (SUCCESS);
}

int	child_process(int idx, int amount_cmds, t_exe *exe, t_node *cmd_node)
{
	set_signals(false);
	if (amount_cmds > 1)
	{
		if (dup_pipes(idx, amount_cmds, 0, exe) == SYS_ERROR)
			return (SYS_ERROR);
	}
	if (ft_lstsize(cmd_node->redir) > 0)
	{
		if (dup_redirect(cmd_node) == SYS_ERROR)
			return (SYS_ERROR);
	}
	if (execute_cmd(cmd_node->cmd, exe) == SYS_ERROR)
		return (SYS_ERROR);
	return (SUCCESS);
}

int	fork_process(int idx, int amount_cmds, t_exe *exe, t_node *cmd_node)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Fork", strerror(errno));
		return (SYS_ERROR);
	}
	else if (pid == 0)
	{
		if (child_process(idx, amount_cmds, exe, cmd_node) == SYS_ERROR)
			return (SYS_ERROR);
	}
	else if (pid > 0)
	{
		if (amount_cmds > 1)
		{
			if (close_pipe_ends(exe->pipe_fds, idx) == SYS_ERROR)
				return (SYS_ERROR);
		}
		if (store_pids(pid, exe) == SYS_ERROR)
			return (SYS_ERROR);
	}
	return (SUCCESS);
}
