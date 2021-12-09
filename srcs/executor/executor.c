/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/18 08:38:10 by annaheister   #+#    #+#                 */
/*   Updated: 2021/11/18 08:38:10 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** DESCRIPTION
**	- Decides to call the piping functions if there is more than 1 cmd
**  or the cmd executor function if it is only one cmd
** JOBS
** 1. Prepares the memory to store the pids of fork(s)
** 2. Prepares the memory to store the pipe_fds if there are 1+ cmds
** 3. Initiates the piping route if there are 1+ cmds
** 4. Initiates the cmd executor route if there is just 1 cmd
*/

void	prepare_execution(t_exe *exe, t_shell *shell)
{
	int	idx;
	int	amount_cmds;

	amount_cmds = ft_lstsize(shell->cmd_nodes);
	exe->pids = malloc(amount_cmds * sizeof(int));
	if (exe->pids == NULL)
		printf("Error - Malloc has failed");
	if (amount_cmds > 1)
	{
		malloc_fds(exe, (amount_cmds - 1));
		pipe_loop(amount_cmds, exe, shell);
	}
	else
		fork_process(0, amount_cmds, exe, shell->cmd_nodes->content);
	idx = 0;
	while (idx < amount_cmds)
	{
		waitpid(exe->pids[idx], NULL, 0);
		idx++;
	}
	free(exe->pids);
}

/*
** DESCRIPTION
**	- Initiates the exe struct, fills it with the environmental path
**  and sends everything to the prepare_execution function
** JOBS
** 1. Prepares the memory for the exe struct
** 2. Takes care of the storage of the environmental path
** 3. All information is send to the prepare_execution function
*/

int	init_exe(t_shell *shell)
{
	t_exe	*exe;

	if (!shell->cmd_nodes)
		return (NONFATAL);
	exe = (t_exe *) malloc(sizeof(t_exe));
	if (!exe)
	{
		printf("Error - Malloc failed\n");
		return (SYS_ERROR);
	}
	exe->paths = NULL;
	exe->envp = environ_to_envp(shell->environ);
	if (init_paths(exe, shell) == SYS_ERROR)	// TODO NONFATAL ??
		printf("Error - Initialization of path failed\n");
	prepare_execution(exe, shell);
	ft_lstclear(&shell->cmd_nodes, node_del);
	return (SUCCESS);
}
