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

#include <exe.h>

/*
** DESCRIPTION
**	- Executes the piping loop. For each cmd the process
**    is forked and the cmd is send to the child process
**    to be executed.
**
** JOBS
** 1. Creates in a loop the pipe fds for each pipe
      reading from the pipe = pipefd[0], writing to the pipe = pipefd[1]
** 2. For each cmd_node the process is forked
** 3. The corresponding cmd content is send to a child process to be executed
** 4. Pids of the children are stored in exe->pids
*/

void pipe_loop(int amount_cmds, t_exe *exe, t_shell *shell)
{
	int 	idx;
	t_node	*cmd_node;
	int 	amount_pipes;

	idx = 0;
	amount_pipes = amount_cmds - 1;
	while (idx < amount_cmds)
	{
		if (idx != amount_pipes)
		{
			if (pipe(exe->pipe_fds[idx]) != 0)
				printf("Error - Creating pipe has failed"); // error handling
		}
		cmd_node = shell->cmd_nodes->content;
		fork_process(idx, amount_cmds, exe, cmd_node);
		shell->cmd_nodes = shell->cmd_nodes->next;
		idx++;
	}
}