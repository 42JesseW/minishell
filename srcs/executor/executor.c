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

void	free_exe(t_exe *exe, t_shell *shell)
{
	ft_strarrfree(&exe->envp);
	ft_lstclear(&exe->pids, free);
	ft_lstclear(&exe->paths, free);
	ft_lstclear(&exe->builtins, free);
	ft_lstclear(&shell->cmd_nodes, node_del);
	free(exe);
}

/*
** DESCRIPTION
**	- Decides to call the piping functions if there is more than 1 cmd
**  or the check_builtin function if it is only one cmd.
** JOBS
** 1. Prepares the memory to store the pids of fork(s)
** 2. Prepares the memory to store the pipe_fds if there are 1+ cmds
** 3. Initiates the piping route if there are 1+ cmds
** 4. Initiates the check_builtin route if there is just 1 cmd
*/

int	wait_process_end(t_shell *shell, t_list	*pid_node)
{
	pid_t	*pid;
	int		w_status;

	(void)shell;
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
			shell->exit_code = 128 + WTERMSIG(w_status);
		}
		if (WIFEXITED(w_status))
			shell->exit_code = WEXITSTATUS(w_status);
		if (g_exit_code_sig)
			g_exit_code_sig = 0;
		pid_node = pid_node->next;
	}
	return (SUCCESS);
}

/*
** DESCRIPTION
**	- Decides to call the piping functions if there is more than 1 cmd
**  or the check_builtin function if it is only one cmd.
** JOBS
** 1. Prepares the memory to store the pids of fork(s)
** 2. Prepares the memory to store the pipe_fds if there are 1+ cmds
** 3. Initiates the piping route if there are 1+ cmds
** 4. Initiates the check_builtin route if there is just 1 cmd
** 5. Waits for all the child_process to finish
*/

int	prepare_execution(t_exe *exe, t_shell *shell)
{
	t_list	*pid_node;

	exe->amount_cmds = ft_lstsize(shell->cmd_nodes);
	if (exe->amount_cmds > 1)
	{
		if (malloc_fds(exe) == SYS_ERROR)
			return (SYS_ERROR);
		if (pipe_loop(exe, shell) == SYS_ERROR)
			return (SYS_ERROR);
	}
	else
	{
		if (builtin_check(0, shell->cmd_nodes->content, exe)
			== SYS_ERROR)
			return (SYS_ERROR);
	}
	pid_node = exe->pids;
	if (wait_process_end(shell, pid_node) == SYS_ERROR)
		return (SYS_ERROR);
	set_signals(true);
	if (exe->amount_cmds > 1)
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
		return (NONFATAL);
	exe = (t_exe *) malloc(sizeof(t_exe));
	if (!exe)
		return (SYS_ERROR);
	exe->paths = NULL;
	exe->builtins = NULL;
	exe->pids = NULL;
	exe->environ = &shell->environ;
	exe->exit_code = &shell->exit_code;
	exe->shell_exit = &shell->shell_exit;
	exe->envp = environ_to_envp(shell->environ);
	if (init_paths(exe, shell) == SYS_ERROR)
		return (SYS_ERROR);
	if (init_builtins(exe) == SYS_ERROR)
		return (SYS_ERROR);
	if (prepare_execution(exe, shell) == SYS_ERROR)
		return (SYS_ERROR);
	free_exe(exe, shell);
	return (SUCCESS);
}
