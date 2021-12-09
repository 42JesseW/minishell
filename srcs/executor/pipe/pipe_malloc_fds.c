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
**	- Mallocs for each pipe in a loop the needed memory for each pipe_fd[2].
*/

void	malloc_fds(t_exe *exe, int amount_pipes)
{
	int	idx;

	exe->pipe_fds = (int **)malloc(sizeof(int *) * (amount_pipes + 1));
	if (!exe->pipe_fds)
		printf("Error - Malloc failed");
	idx = 0;
	while (idx < amount_pipes)
	{
		exe->pipe_fds[idx] = (int *) malloc(sizeof(int) * 2);
		if (!exe->pipe_fds[idx])
		{
			printf("Error - Malloc failed");
			break ;
		}
		idx++;
	}
	exe->pipe_fds[amount_pipes] = NULL;
}
