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

// TODO - Keys met value ook handlen
// TODO - Check unset in combinatie met pipes

#include <minishell.h>

int	check_all_alpha(char *cmd)
{
	int		idx;
	char	*message;

	idx = 0;
	while (idx < (int)ft_strlen(cmd))
	{
		if (ft_isalpha(cmd[idx]) == 0 && cmd[idx] != '_')
		{
			message = ft_strnjoin(3, "unset: '", cmd, "'");
			ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, message,
				"not a valid identifier");
			free (message);
			return (EXIT_FAILURE);
		}
		idx++;
	}
	return (EXIT_SUCCESS);
}

void	key_loop(char **cmd, t_exe *exe)
{
	int		idx;
	t_pair	*pair;
	t_list	*node;

	idx = 1;
	while (idx < ft_strarrlen(cmd))
	{
		if (check_all_alpha(cmd[idx]) == EXIT_SUCCESS)
		{
			node = *exe->environ;
			while (node)
			{
				pair = node->content;
				if (ft_strncmp(pair->key, cmd[idx], ft_strlen(cmd[idx])) == 0)
				{
					ft_lstunlink(exe->environ, node);
					ft_lstdelone(node, pair_del);
					break ;
				}
				else
					node = node->next;
			}
		}
		idx++;
	}
}

/*
** DESCRIPTION
**	-  The function 'unset' unsets values and attributes of variables
**     and functions of the environmental variables
*/

int	builtin_unset(char **cmd, t_exe *exe)
{
	if (ft_strarrlen(cmd) < 2)
	{
		if (ft_strarrlen(cmd) == 1)
			ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "unset",
				"not enough arguments");
		return (SUCCESS);
	}
	key_loop(cmd, exe);
	return (EXIT_SUCCESS);
}
