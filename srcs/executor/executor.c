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

int	prepare_execution(t_exe *exe, t_shell *shell) // TODO Functie splitsen - te lang
{
	int	idx;
	int status;
	int	amount_cmds;

	amount_cmds = ft_lstsize(shell->cmd_nodes);
	exe->pids = malloc(amount_cmds * sizeof(int));
	if (exe->pids == NULL)
	{
		dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Malloc", strerror(errno));
		return (SYS_ERROR);
	}
	if (amount_cmds > 1)
	{
		if (malloc_fds(exe, (amount_cmds - 1)) == SYS_ERROR)
			return (SYS_ERROR);
		if (pipe_loop(amount_cmds, exe, shell) == SYS_ERROR) // Nog functie aanpassen: int
			return (SYS_ERROR);
	}
	else
		if (fork_process(0, amount_cmds, exe, shell->cmd_nodes->content) == SYS_ERROR) // Nog functie aanpassen: int
			return (SYS_ERROR);
	idx = 0;
	while (idx < amount_cmds)
	{
		if (waitpid(exe->pids[idx], &status, 0) == -1) //UITZOEKEN
		{
			dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Child process ended with", WEXITSTATUS(status)) // TODO waitpid uitzoeken waarde WEXITSTATUS
			return (SYS_ERROR);
		}
		idx++;
	}
	free(exe->pids);
	// Hier nog fds freeen
	return (SUCCESS);
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
	{
		dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Input", "cmd_nodes = NULL");
		return (NONFATAL);
	}
	exe = (t_exe *) malloc(sizeof(t_exe));
	if (!exe)
	{
		dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Malloc", strerror(errno));
		return (SYS_ERROR);
	}
	exe->paths = NULL;
	exe->envp = environ_to_envp(shell->environ);
	if (init_paths(exe, shell) == SYS_ERROR)	// TODO NONFATAL ??
		return (SYS_ERROR);
	prepare_execution(exe, shell);
	ft_lstclear(&shell->cmd_nodes, node_del);
	return (SUCCESS);
}
