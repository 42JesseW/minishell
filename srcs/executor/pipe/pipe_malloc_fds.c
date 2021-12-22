/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe_malloc_fds.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/27 15:10:51 by annaheister   #+#    #+#                 */
/*   Updated: 2021/11/27 15:10:51 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** DESCRIPTION
**	- Mallocs for each pipe in a loop the needed memory for both pipe ends.
*/

int	malloc_fds(t_exe *exe, int amount_pipes)
{
	int	idx;

	exe->pipe_fds = (int **)malloc(sizeof(int *) * (amount_pipes + 1));
	if (!exe->pipe_fds)
	{
		dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Malloc", strerror(errno));
		return (SYS_ERROR);
	}
	idx = 0;
	while (idx < amount_pipes)
	{
		exe->pipe_fds[idx] = (int *) malloc(sizeof(int) * 2);
		if (!exe->pipe_fds[idx])
		{
			dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Malloc",
				strerror(errno));
			return (SYS_ERROR);
		}
		idx++;
	}
	exe->pipe_fds[amount_pipes] = NULL;
	return (SUCCESS);
}
