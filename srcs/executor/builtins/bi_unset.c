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

// TODO unlinken van node gaat fout
// TODO PATH* - unset: `PATH*': not a valid identifier

#include <minishell.h>

/*
** DESCRIPTION
**	-  The function 'unset' unsets values and attributes of variables
**     and functions of the environmental variables
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
	node = *exe->environ;
	while (node)
	{
		pair = node->content;
		if (ft_strncmp(pair->key, cmd[1], ft_strlen(cmd[1])) == 0)
		{
			ft_lstunlink(exe->environ, node);
			ft_lstdelone(node, pair_del);
			return (SUCCESS);
		}
		else
			node = node->next;
	}
	return (EXIT_SUCCESS);
}
