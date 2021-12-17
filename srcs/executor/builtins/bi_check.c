/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bi_check.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/17 10:16:40 by annaheister   #+#    #+#                 */
/*   Updated: 2021/12/17 10:16:40 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	builtin_check(int idx, int amount_cmds, t_node *cmd_node, t_exe *exe)
{
	int 		idj;
	t_builtin	*builtin;

	idj = 0;
	while (idj < ft_lstsize(exe->builtins))
	{
		builtin = exe->builtins->content;
		if (ft_strcmp(cmd_node->cmd[0], builtin->name) == 0)
		{
			if (ft_lstsize(cmd_node->redir) > 0)
			{
				if (dup_redirect(cmd_node) == SYS_ERROR)
					return (SYS_ERROR);
			}
			if ((*builtin->function)(cmd_node) == SYS_ERROR)
				return (SYS_ERROR);
			else
				return (SUCCESS);
		}
		exe->builtins = exe->builtins->next;
		idj++;
	}
	if (fork_process(idx, amount_cmds, exe, cmd_node)
		== SYS_ERROR)
		return (SYS_ERROR);
	return (SUCCESS);
}
