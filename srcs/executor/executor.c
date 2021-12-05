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

# include <exe.h>

/*
** DESCRIPTION
**	- Prepares the memory for executing the piping loop
**    and cleans up after the loop
** JOBS
** 1. Prepares the memory for the pipe fds and the pids
** 2. Initiates the pipe_loop
** 3. Waits for the terminated children in the pipe_loop
** 4. Frees the memory for the pipe fds and the pids
*/

void prepare_execution(t_exe *exe, t_shell *shell)
{
	int 	idx;
	int 	amount_cmds;

	amount_cmds = ft_lstsize(shell->cmd_nodes);
	malloc_fds(exe, amount_cmds);
	exe->pids = malloc(amount_cmds * sizeof(int));
	if (exe->pids == NULL)
		printf("Error - Malloc has failed"); // error handling
	if (amount_cmds > 1)
		pipe_loop(amount_cmds, exe, shell);
	else
		fork_process(0, amount_cmds, exe, shell->cmd_nodes->content);
	idx = 0;
	while (idx < amount_cmds) // hier gaat er iets fout bij 3: hij loopt niet verder dan 1
	{
		waitpid(exe->pids[idx], NULL, 0); // later in aparte functie ivm exitcodes
		idx++;
	}
	free(exe->pids);
	//Hier nog een free array voor de pipe_fds
}

/*
** DESCRIPTION
**	- Initiates the exe struct, fills it with the environmental path,
**  and decides to call the piping functions if there is more than 1 cmd
**  or the cmd executor function if it is only one cmd
** JOBS
** 1. Prepares the memory for the exe struct
** 2. Takes care of the storage of the environmental path
** 3. Initiates the piping route if there are 1+ cmds
** 4. Initiates the cmd executor route if there is just 1 cmd
*/

void init_exe(t_shell *shell)
{
	t_exe 	*exe;

	exe = (t_exe *) malloc(sizeof(t_exe));
	if (!exe)
		printf("Error - Malloc failed\n");
	exe->paths = NULL;
	exe->envp = environ_to_envp(shell->environ);
	if (init_paths(exe, shell) == SYS_ERROR)
		printf("Error - Initialization of path failed\n");
	prepare_execution(exe, shell);
}

int	main(int argc, char **argv, const char *envp[]) //temp
{
	t_shell	*shell;
	t_list	*environ;
	t_node	*cmd_node;
	int 	idx;
//	char 	*cmd[1][3] = {
//			{"echo", "cat", NULL}
//	};
//		char 	*cmd[2][3] = {
//			{"ls", "-l", NULL},
//			{"wc", "-l", NULL}
//	};
//	char 	*cmd[3][3] = {
//			{"ls", "-l", NULL},
//			{"wc", "-l", NULL},
//			{"echo", "cat", NULL},
//	};
	char 	*cmd[4][3] = {
			{"ls", "-l", NULL},
			{"wc", "-l", NULL},
			{"echo", "cat", NULL},
			{"wc", "-l", NULL},
	};

	idx = 0;
	environ = NULL;
	shell = (t_shell *) malloc(sizeof(t_shell));
	environ_from_envp(&environ, (const char **) envp);
	shell->environ = environ;
	shell->cmd_nodes = NULL;
	while (idx < 4)
	{
		cmd_node = (t_node *) malloc(sizeof(t_node));
		cmd_node->cmd = cmd[idx];
		cmd_node->redir = NULL;
		ft_lstadd_back(&shell->cmd_nodes, ft_lstnew(cmd_node));
		idx++;
	}
	init_exe(shell);
	return (0);
}

//gcc -Iincludes -Ilibft/includes -Llibft -lft srcs/executor/dup/dup.c srcs/executor/dup/dup_cmd.c srcs/executor/dup/dup_pipes.c srcs/executor/execute/execute.c srcs/executor/fork/fork.c srcs/executor/pipe/pipe.c srcs/executor/pipe/pipe_close.c srcs/executor/pipe/pipe_malloc_fds.c srcs/executor/path/path_of_cmd.c srcs/executor/path/path_store.c srcs/executor/executor.c srcs/shell/environ/environ_from_envp.c srcs/shell/environ/environ_to_envp.c srcs/shell/environ/pair/pair_new.c srcs/shell/environ/pair/pair_join.c srcs/shell/environ/pair/pair_del.c

