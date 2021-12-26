/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exe_cmd.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/18 14:11:04 by annaheister   #+#    #+#                 */
/*   Updated: 2021/11/18 14:11:04 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** DESCRIPTION
**	- Finds the path to the folder from where the cmd can be executed
**    and executes it
** JOBS
** 1. Finds the path to the folder from where the cmd can be executed
** 2. Executes the cmd
** 3. Frees the path if the execution fails (otherwise the child process exits
**    and automatically frees all memory)
*/

void	execute_cmd(char **cmd, t_exe *exe)
{
	char	*path;

	path = get_full_path(cmd[0], exe);
	if (path == NULL)
		exit (EXIT_FAILURE);
	if (execve(path, cmd, exe->envp) == -1)
	{
		free(path);
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "execution",
			strerror(errno));
		exit (EXIT_FAILURE);
	}
}
