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

int	dup_pipe_write(int idx, t_exe *exe)
{
	int	fd_out;

	fd_out = dup2(exe->pipe_fds[idx][1], STDOUT_FILENO);
	if (close(exe->pipe_fds[idx][1]) || fd_out == -1)
	{
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Dup", strerror(errno));
		return (SYS_ERROR);
	}
	return (SUCCESS);
}

int	dup_pipe_read(int idx, t_exe *exe)
{
	int	fd;

	fd = dup2(exe->pipe_fds[idx][0], STDIN_FILENO);
	if (close(exe->pipe_fds[idx][0]) == -1 || fd == -1)
	{
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Malloc",
			strerror(errno));
		return (SYS_ERROR);
	}
	return (SUCCESS);
}

/*
** DESCRIPTION
**	- creates a copy for the fds of the writing or the reading side of
**    the pipe (depending on the position of the cmd: left (write) or
8*    right (read) from the pipe).
** JOBS
** 1. Checks if a cmd is not the last one of the list (in that case write)
** 2. AANVULLEN
*/

int	dup_pipes(int idx, int amount_cmds, t_exe *exe)
{
	if (idx != (amount_cmds - 1))
		if (dup_pipe_write(idx, exe) == SYS_ERROR)
			return (SYS_ERROR);
	if (idx != 0)
	{
		idx--;
		if (dup_pipe_read(idx, exe) == SYS_ERROR)
			return (SYS_ERROR);
	}
//	if (close(exe->pipe_fds[idx][0]) == -1
//		|| close(exe->pipe_fds[idx][1]) == -1)
//	{
//		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Close pipe",
//			strerror(errno));
//		return (SYS_ERROR);
//	}
	return (SUCCESS);
}
