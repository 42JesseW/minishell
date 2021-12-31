/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bi_env.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/22 16:03:01 by annaheister   #+#    #+#                 */
/*   Updated: 2021/12/22 16:03:01 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** DESCRIPTION
**	-  Prints the environmental variables in the order in which the
**     variables are stored. Prints only the environmental variables
**     that have both a key and a value.
*/

int	builtin_env(char **cmd, t_exe *exe)
{
	t_pair	*pair;
	t_list	*node;

	(void)cmd;
	if (ft_strarrlen(cmd) > 1)
	{
		printf("env: %s: No such file or directory\n", cmd[1]);
		return (SUCCESS);
	}
	node = *exe->environ;
	while (node)
	{
		pair = node->content;
		if (pair->val)
		{
			ft_printf("%s=", pair->key);
			ft_printf("%s\n", pair->val);
		}
		node = node->next;
	}
	return (SUCCESS);
}
