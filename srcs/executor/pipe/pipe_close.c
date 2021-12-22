/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe_close.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/27 15:12:32 by annaheister   #+#    #+#                 */
/*   Updated: 2021/11/27 15:12:32 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** DESCRIPTION
**	- Closes the (unused) pipe_ends of the parent
*/

int	close_pipe_ends(int **pipes_fds, int idx)
{
	if (idx != 0)
	{
		idx--;
		if (close(pipes_fds[idx][0]) == -1 || close(pipes_fds[idx][1]) == -1)
		{
			ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Close pipe",
				strerror(errno));
			return (SYS_ERROR);
		}
	}
	return (SUCCESS);
}
