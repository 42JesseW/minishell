/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   dup_pipes.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/02 19:21:54 by annaheister   #+#    #+#                 */
/*   Updated: 2021/12/02 19:21:54 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// hier moeten de redirects nog ingeschreven worden

# include <exe.h>

void	dup_pipes(int idx, int amount_cmds, t_exe *exe, t_node *cmd_node)
{
	if (idx != (amount_cmds - 1)) // als het niet het laatste cmd is
		dup_pipe_write(idx, exe);
	else if (idx != 0) // als het niet het eerste cmd is
	{
		idx--;
		dup_pipe_read(idx, exe);
	}
	close(exe->pipe_fds[idx][0]);
	close(exe->pipe_fds[idx][1]);
	execute_cmd(cmd_node->cmd, exe);
}
