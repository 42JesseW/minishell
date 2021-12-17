/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/16 09:48:18 by aheister      #+#    #+#                 */
/*   Updated: 2021/12/16 09:48:18 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// TODO probleem met fd redir oplossen

#include <minishell.h>

/*
** DESCRIPTION
**	-  Moet nog aangevuld
*/

int	check_n(char *cmd)
{
	int	idx;
	int	len;

	idx = 1;
	len = ft_strlen(cmd);
	while (idx < len)
	{
		if (cmd[idx] != 'n')
			return (0);
		idx++;
	}
	return (SUCCESS);
}

int	builtin_echo(t_node *cmd_node)
{
	int	idx;
	int	len;
	int	flag_n;
	int fd;

	idx = 1;
	flag_n = 0;
	fd = dup(STDOUT_FILENO);
	len = ft_strarrlen(cmd_node->cmd);
	if (ft_strnstr(cmd_node->cmd[1], "-n", 2) > 0)
	{
		flag_n = check_n(cmd_node->cmd[1]);
		if (flag_n == SUCCESS)
			idx++;
	}
	while (idx < len)
	{
		dprintf(fd, "%s", cmd_node->cmd[idx]);
		if (idx < len - 1)
			dprintf(fd, " ");
		idx++;
	}
	if (flag_n == 0)
		dprintf(fd, "\n");
	return (SUCCESS);
}
