/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bi_exit.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/30 08:14:06 by annaheister   #+#    #+#                 */
/*   Updated: 2021/12/30 08:14:06 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	builtin_exit(char **cmd, t_exe *exe)
{
	(void)cmd;
	ft_printf("exit\n");
	//*exe->exit_code = 256;
	*exe->shell_exit = 1;
	return (SUCCESS);
}
