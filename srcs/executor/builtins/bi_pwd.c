/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bi_pwd.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/22 19:35:10 by annaheister   #+#    #+#                 */
/*   Updated: 2021/12/22 19:35:10 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include <minishell.h>

/*
** DESCRIPTION
**	-  Prints the name of the current working directory.
*/

int	builtin_pwd(char **cmd, t_exe *exe)
{
	char	buff[PATH_MAX];

	(void)cmd;
	(void)exe;
	if (getcwd(buff, sizeof(buff)) == NULL)
		return (SYS_ERROR);
	else
		ft_printf("%s\n", buff);
	return (SUCCESS);
}
