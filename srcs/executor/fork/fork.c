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

int	fork_process(int idx, int amount_cmds, t_exe *exe, t_node *cmd_node)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Fork", strerror(errno));
		return (SYS_ERROR);
	}
	else if (pid == 0)
	{
		if (amount_cmds > 1)
			if (dup_pipes(idx, amount_cmds, exe, cmd_node) == SYS_ERROR) // functie aanpassen
				return (SYS_ERROR);
		else
			if (dup_cmd(exe, cmd_node) == SYS_ERROR) // functie aanpassen
				return (SYS_ERROR);
	}
	else if (pid > 0)
	{
		exe->pids[idx] = pid;
		if (amount_cmds > 1)
			if (close_pipe_ends(exe->pipe_fds, idx) == == SYS_ERROR) // TODO checken of close een error kan genereren, zo ja functie aanpassen
				return (SYS_ERROR);
	}
	return (SUCCESS);
}
