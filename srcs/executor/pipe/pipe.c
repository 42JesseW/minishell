/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exe_pipe.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 12:53:05 by aheister      #+#    #+#                 */
/*   Updated: 2021/11/12 12:53:05 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** DESCRIPTION
**	- Executes the piping loop. For each cmd the process
**    is forked and the cmd is send to the fork_process
**    to be executed.
** JOBS
** 1. Creates in a loop the pipe fds for each pipe
      reading from the pipe = pipefd[0], writing to the pipe = pipefd[1]
** 2. Each cmd node is send to the forking process to be executed
*/

int	pipe_loop(t_exe *exe, t_shell *shell)
{
    t_list  *cmd_node;
	int		idx;
	int		amount_pipes;

	idx = 0;
    cmd_node = shell->cmd_nodes;
	amount_pipes = exe->amount_cmds - 1;
	while (idx < exe->amount_cmds)
	{
		if (idx != amount_pipes)
		{
			if (pipe(exe->pipe_fds[idx]) != 0)
				return (SYS_ERROR);
		}
		if (builtin_check(idx, cmd_node->content, exe)
			== SYS_ERROR)
			return (SYS_ERROR);
		cmd_node = cmd_node->next;
		idx++;
	}
	return (SUCCESS);
}
