/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exe.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/18 07:57:08 by annaheister   #+#    #+#                 */
/*   Updated: 2021/11/18 07:57:08 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_H
# define EXE_H

# include <minishell.h>
# include <unistd.h>
# include <fcntl.h>

typedef struct s_exe
{
	t_list	*paths;
	int		**pipe_fds;
	pid_t 	*pids;
	int 	fd_in;
	int 	fd_out;
	char 	**envp; //temp
}	t_exe;

// INITIALISATION
void	init_exe(t_shell *shell);
int		init_paths(t_exe *exe, t_shell *shell);
void	store_paths(const char *strpaths, t_exe *exe);
void	prepare_execution(t_exe *exe, t_shell *shell);

// PIPING
void 	malloc_fds(t_exe *exe, int amount_cmds);
void	pipe_loop(int amount_cmds, t_exe *exe, t_shell *shell);


// DUPPING
void	dup_cmd(t_exe *exe, t_node *cmd_node);
void	dup_pipes(int idx, int amount_cmds, t_exe *exe, t_node *cmd_node);
void	dup_pipe_write(int idx, t_exe *exe);
void	dup_pipe_read(int idx, t_exe *exe);
void	dup_redirect_write(char *file);
void	dup_redirect_read(char *file);

// FORKING
void 	fork_process(int idx, int amount_cmds, t_exe *exe, t_node *cmd_node);
void	close_pipe_ends(int **pipes_fds, int idx);

// EXECUTION
char 	*get_full_path(char *cmd, t_exe *exe);
void	execute_cmd(char **cmd, t_exe *exe);

#endif
