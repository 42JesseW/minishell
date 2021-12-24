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

int	builtin_check(int idx, int amount_cmds, t_node *node, t_exe *exe)
{
	bool		is_builtin;
	int			old_stdout;
	int			old_stdin;
	t_list		*bi_node;
	t_builtin	*builtin;

	bi_node = exe->builtins;
	old_stdout = dup(STDOUT_FILENO);
	old_stdin = dup(STDIN_FILENO);
	while (bi_node)
	{
		builtin = bi_node->content;
		if (ft_strcmp(node->cmd[0], builtin->name) == 0)
		{
			is_builtin = 1;
			if (amount_cmds > 1)
			{
				if (dup_pipes(idx, amount_cmds, is_builtin, exe) == SYS_ERROR)
					return (SYS_ERROR);
			}
			if (ft_lstsize(node->redir) > 0)
			{
				if (dup_redirect(node) == SYS_ERROR)
					return (SYS_ERROR);
			}
			if ((*builtin->function)(node->cmd, exe) == SYS_ERROR)
				return (SYS_ERROR);
			else
			{
				dup2(old_stdout, STDOUT_FILENO);
				dup2(old_stdin, STDIN_FILENO);
				close(old_stdout);
				close(old_stdin);
				return (SUCCESS);
			}
		}
		bi_node = bi_node->next;
	}
	if (fork_process(idx, amount_cmds, exe, node) == SYS_ERROR)
		return (SYS_ERROR);
	return (SUCCESS);
}
