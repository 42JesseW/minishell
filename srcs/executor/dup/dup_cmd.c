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

#include <minishell.h>

/*
** DESCRIPTION
**	-  Moet nog aangevuld worden voor situatie met redirects
*/

int	dup_cmd(t_exe *exe, t_node *cmd_node)
{
	if (execute_cmd(cmd_node->cmd, exe) == SYS_ERROR)
		return (SYS_ERROR);
	return (SUCCESS);
}
