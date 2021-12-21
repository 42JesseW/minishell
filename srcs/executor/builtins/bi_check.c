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

int	builtin_check(char **cmd, t_exe *exe)
{
	int			len;
	t_builtin	*builtin;

	len = ft_lstsize(exe->builtins);
	while (len > 0)
	{
		builtin = exe->builtins->content;
		if (ft_strcmp(cmd[0], builtin->name) == 0)
		{
			if ((*builtin->function)(cmd) == SYS_ERROR)
				return (SYS_ERROR);
			else
				return (SUCCESS);
		}
		exe->builtins = exe->builtins->next;
		len--;
	}
	return (0);
}
