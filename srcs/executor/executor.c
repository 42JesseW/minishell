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

// TODO - Overal alles veilig maken en op goede moment alles freeen
// TODO - Overal printf en dprintf vervangen door ft_printf etc.
// TODO - Aantal grote functies splitsen
// TODO - WEXITSTATUS uitzoeken
// TODO - Overal descriptions bij maken
// TODO - Tests schrijven voor een aantal functies
// TODO - In execute.c uitzoeken hoe ik het beste de functie kan verlaten

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

void	free_exe(t_exe *exe, t_shell *shell)
{
	ft_lstclear(&exe->paths, free);
	ft_strarrfree(&exe->envp);
	ft_lstclear(&shell->cmd_nodes, node_del);
	free(exe);
}

int	prepare_execution(t_exe *exe, t_shell *shell) // TODO Functie splitsen - te lang
{
	int		len;
	int		status;
	int		amount_cmds;
	pid_t	*pid;

	amount_cmds = ft_lstsize(shell->cmd_nodes);
	if (amount_cmds > 1)
	{
		if (malloc_fds(exe, (amount_cmds - 1)) == SYS_ERROR)
			return (SYS_ERROR);
		if (pipe_loop(amount_cmds, exe, shell) == SYS_ERROR)
			return (SYS_ERROR);
	}
	else
	{
		if (fork_process(0, amount_cmds, exe, shell->cmd_nodes->content)
			== SYS_ERROR)
			return (SYS_ERROR);
	}
	len = ft_lstsize(exe->pids);
	while (len > 0)
	{
		pid = exe->pids->content;
		if (waitpid(*pid, &status, 0) == -1)
		{
			//dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Child process ended with", WEXITSTATUS(status)); // TODO waitpid uitzoeken waarde WEXITSTATUS
			return (SYS_ERROR);
		}
		exe->pids = exe->pids->next;
		len--;
	}
	if (amount_cmds > 1)
		free_pipe_fds(exe->pipe_fds);
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
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Input", "cmd_nodes = NULL");
		return (NONFATAL);
	}
	exe = (t_exe *) malloc(sizeof(t_exe));
	if (!exe)
	{
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Malloc", strerror(errno));
		return (SYS_ERROR);
	}
	exe->paths = NULL;
	exe->builtins = NULL;
	exe->pids = NULL;
	exe->envp = environ_to_envp(shell->environ);	// TODO deze functie beschermen (SYS_ERROR)
	if (init_paths(exe, shell) == SYS_ERROR)
		return (SYS_ERROR);
	init_builtins(exe);								// TODO deze functie beschermen (SYS_ERROR)
	prepare_execution(exe, shell);					// TODO deze functie beschermen (SYS_ERROR)
	free_exe(exe, shell);							// TODO deze functie beschermen (SYS_ERROR)
	return (SUCCESS);
}
