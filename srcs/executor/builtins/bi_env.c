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
**	-  Prints the environmental variables in the order in which the variables are stored.
**     Prints only the environmental variables that have both a key and a value
*/

int	builtin_env(char **cmd, t_exe *exe)
{
	int	idx;
	int	len;

	(void)cmd;
	idx = 0;
	len = ft_strarrlen(exe->envp);
	while (idx < len)
	{
		ft_printf("%s\n", exe->envp[idx]);
		idx++;
	}
	return (SUCCESS);
}
