/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   dup_cmd.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/02 19:21:21 by annaheister   #+#    #+#                 */
/*   Updated: 2021/12/02 19:21:21 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// hier moeten nog de redirects ingeschreven worden

# include <exe.h>

void	dup_cmd(t_exe *exe, t_node *cmd_node)
{
//	exe->fd_in = dup(STDIN_FILENO);
//	exe->fd_out = dup(STDOUT_FILENO);
	execute_cmd(cmd_node->cmd, exe);
}

