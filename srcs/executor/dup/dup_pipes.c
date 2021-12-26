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

int	dup_pipe_write(int idx, int is_builtin, t_exe *exe)
{
	int	fd_out;

	fd_out = dup2(exe->pipe_fds[idx][1], STDOUT_FILENO);
	if (is_builtin == 1)
	{
		if (close(exe->pipe_fds[idx][1]) == -1)
		{
			ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "close pipe",
				strerror(errno));
			return (SYS_ERROR);
		}
	}
	if (fd_out == -1)
	{
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "dup", strerror(errno));
		return (SYS_ERROR);
	}
	return (SUCCESS);
}

int	dup_pipe_read(int idx, int is_builtin, t_exe *exe)
{
	int	fd_in;

	fd_in = dup2(exe->pipe_fds[idx][0], STDIN_FILENO);
	if (is_builtin == 1)
	{
		if (close(exe->pipe_fds[idx][0]) == -1)
		{
			ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "close pipe",
				strerror(errno));
			return (SYS_ERROR);
		}
	}
	if (fd_in == -1)
	{
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "dup", strerror(errno));
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

int	dup_pipes(int idx, int is_builtin, t_exe *exe)
{
	if (idx != (exe->amount_cmds - 1))
		if (dup_pipe_write(idx, is_builtin, exe) == SYS_ERROR)
			return (SYS_ERROR);
	if (idx != 0)
	{
		idx--;
		if (dup_pipe_read(idx, is_builtin, exe) == SYS_ERROR)
			return (SYS_ERROR);
	}
	if (is_builtin == 0)
	{
		if (close(exe->pipe_fds[idx][0]) == -1
			|| close(exe->pipe_fds[idx][1]) == -1)
		{
			ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "close pipe",
				strerror(errno));
			return (SYS_ERROR);
		}
	}
	return (SUCCESS);
}
