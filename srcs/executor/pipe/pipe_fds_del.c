/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe_free_fds.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/05 16:18:31 by aheister      #+#    #+#                 */
/*   Updated: 2021/12/05 16:18:31 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** DESCRIPTION
**	- Frees the allocated memory for the pipe_fds
*/

void	free_pipe_fds(int **pipe_fds)
{
	int	idx;

	idx = 0;
	while (pipe_fds[idx])
	{
		free(pipe_fds[idx]);
		idx++;
	}
	free(pipe_fds);
}
