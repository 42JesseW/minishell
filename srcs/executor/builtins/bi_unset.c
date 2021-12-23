/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bi_unset.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/23 11:25:45 by annaheister   #+#    #+#                 */
/*   Updated: 2021/12/23 11:25:45 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// checken of key bestaat en of key valid is
// bij PATH* - unset: `PATH*': not a valid identifier

/*
** DESCRIPTION
**	-  VOLGT
*/

int	builtin_unset(char **cmd, t_exe *exe)
{
	t_pair	*pair;
	t_list	*node;

	if (ft_strarrlen(cmd) < 2 || ft_strarrlen(cmd) > 2)
	{
		if (ft_strarrlen(cmd) == 1)
			ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "unset",
				"not enough arguments");
		return (SUCCESS);
	}
	node = exe->environ;
	while (node)
	{
		pair = node->content;
		if (ft_strncmp(pair->key, cmd[1], ft_strlen(cmd[1])) == 0)
		{
			printf("cmd = %s\n", cmd[1]);
			ft_lstunlink(&exe->environ, node);
			ft_lstdelone(node, pair_del);
			return (SUCCESS);
		}
		else
			node = node->next;
	}
	return(EXIT_SUCCESS);
}
