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
**	- Mallocs in a loop the needed memory for each pipe fd[2]
*/

void 	malloc_fds(t_exe *exe, int amount_cmds)
{
	int	idx;

	exe->pipe_fds = (int **)malloc(sizeof(int *) * amount_cmds);
	if (!exe->pipe_fds)
		printf("Error - Malloc failed"); // error handling
	idx = 0;
	while (idx < (amount_cmds - 1))
	{
		exe->pipe_fds[idx] = (int *) malloc(sizeof(int) * 2);
		if (!exe->pipe_fds[idx])
		{
			printf("Error - Malloc failed");
			break; // error handling
		}
		idx++;
	}
	exe->pipe_fds[amount_cmds] = NULL;
}
