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

#include <minishell.h>

void	dup_pipe_write(int idx, t_exe *exe)
{
	int	fd_out;

	fd_out = dup2(exe->pipe_fds[idx][1], STDOUT_FILENO);
	if (fd_out == -1)
		printf("Error - Duplicating fd failed");
}

void	dup_pipe_read(int idx, t_exe *exe)
{
	int	fd;

	fd = dup2(exe->pipe_fds[idx][0], STDIN_FILENO);
	if (fd == -1)
		printf("Error - Duplicating fd failed");
}

/*
** DESCRIPTION
**	- creates a copy for the fds of the writing or the reading side of
**    the pipe (depending on the position of the cmd: left (write) or
8*    right (read) from the pipe).
** JOBS
** 1. Checks if a cmd is not the last one of the list (in that case write)
** 2. Each cmd node is send to the forking process to be executed
*/

void	dup_pipes(int idx, int amount_cmds, t_exe *exe, t_node *cmd_node)
{
	if (idx != (amount_cmds - 1))
		dup_pipe_write(idx, exe);
	if (idx != 0)
	{
		idx--;
		dup_pipe_read(idx, exe);
	}
	close(exe->pipe_fds[idx][0]);
	close(exe->pipe_fds[idx][1]);
	execute_cmd(cmd_node->cmd, exe);
}
