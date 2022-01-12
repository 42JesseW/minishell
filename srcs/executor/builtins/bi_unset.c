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

static void	unset(t_list **environ, char *key)
{
	t_list	*node;
	t_pair	*pair;

	node = *environ;
	while (node)
	{
		pair = (t_pair *)node->content;
		if (ft_strcmp(pair->key, key) == 0)
		{
			ft_lstunlink(environ, node);
			ft_lstdelone(node, pair_del);
			break ;
		}
		node = node->next;
	}
}

int	builtin_unset(char **cmd, t_exe *exe)
{
	int	exit_code;
	int	idx;

	if (!cmd || !cmd[0] || ft_strcmp(cmd[0], "unset") != 0)
		return (EXIT_FAILURE);
	idx = 1;
	exit_code = EXIT_SUCCESS;
	while (cmd[idx])
	{
		if (is_valid_key(cmd[idx]))
			unset(exe->environ, cmd[idx]);
		else
			invalid_key_msg(cmd[idx], &exit_code);
		idx++;
	}
	return (exit_code);
}
