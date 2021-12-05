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

void	fork_process(int idx, int amount_cmds, t_exe *exe, t_node *cmd_node)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		printf("Error - Fork has failed");
	else if (pid == 0)
	{
		if (amount_cmds > 1)
			dup_pipes(idx, amount_cmds, exe, cmd_node);
		else
			dup_cmd(exe, cmd_node);
	}
	else if (pid > 0)
	{
		exe->pids[idx] = pid;
		if (amount_cmds > 1)
			close_pipe_ends(exe->pipe_fds, idx);
	}
}
