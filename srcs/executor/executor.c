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
// TODO - Aantal grote functies splitsen
// TODO - WEXITSTATUS uitzoeken
// TODO - Overal descriptions bij maken
// TODO - Tests schrijven voor een aantal functies
// TODO - In execute.c uitzoeken hoe ik het beste de functie kan verlaten
// TODO - Functie prepare_execution splitsen
// TODO - In init_exe zijn er nog verschillende functies niet beschermd
// TODO - Uitzoeken wat launch van cmd by using relative or absolute path betekent
// TODO - Uitzoeken of eindigen met ctrl-C, ctrl-D en ctrl-\ werkt
// 		- ctrl-\ (CTRL + \) doet niks in parent. Wanneer in child uitgevoerd,
//		  opvangen met WIFSIGNALED(w_status) waar w_status komt uit waitpid(pid, &w_status, 0)
//		  met WTERMSIG(w_status) kan gekeken worden welke signal. SIGQUIT (ctrl-\) is 3.
// TODO - History doet soms gek (proberen te reproduceren)

#include <minishell.h>

void	free_exe(t_exe *exe, t_shell *shell)
{
	ft_lstclear(&exe->paths, free);
	ft_strarrfree(&exe->envp);
	ft_lstclear(&shell->cmd_nodes, node_del);
	free(exe);
}

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

int	prepare_execution(t_exe *exe, t_shell *shell)
{
	pid_t	*pid;
	int		w_status;
	t_list	*pid_node;
	int		amount_cmds;

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
		if (builtin_check(0, amount_cmds, shell->cmd_nodes->content, exe)
			== SYS_ERROR)
			return (SYS_ERROR);
//		if (fork_process(0, amount_cmds, exe, shell->cmd_nodes->content)
//			== SYS_ERROR)
//			return (SYS_ERROR);
	}
	pid_node = exe->pids;
	while (pid_node)
	{
		pid = pid_node->content;
		if (waitpid(*pid, &w_status, 0) == -1)
			return (SYS_ERROR);
		if (!pid_node->next && WIFSIGNALED(w_status))
		{
			if (WTERMSIG(w_status) == SIGQUIT)
				ft_dprintf(STDERR_FILENO, "Quit: %d", WTERMSIG(w_status));
			ft_dprintf(STDERR_FILENO, "\n");
		}
		if (WIFEXITED(w_status))
			ft_dprintf(STDERR_FILENO, "error code: %d\n", WEXITSTATUS(w_status));	// TODO change to setting exit_code
		pid_node = pid_node->next;
	}
	set_signals(true);
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
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Input",
			"no input");
		return (NONFATAL);
	}
	exe = (t_exe *) malloc(sizeof(t_exe));
	if (!exe)
	{
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Malloc",
			strerror(errno));
		return (SYS_ERROR);
	}
	exe->paths = NULL;
	exe->builtins = NULL;
	exe->pids = NULL;
	exe->environ = shell->environ;
	exe->envp = environ_to_envp(shell->environ);
	if (init_paths(exe, shell) == SYS_ERROR)
		return (SYS_ERROR);
	init_builtins(exe);
	prepare_execution(exe, shell);
	free_exe(exe, shell);
	return (SUCCESS);
}
