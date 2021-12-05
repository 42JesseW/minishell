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

void	close_pipe_ends(int **pipes_fds, int idx)
{
	if (idx != 0)
	{
		idx--;
		close(pipes_fds[idx][0]);
		close(pipes_fds[idx][1]);
	}
}
