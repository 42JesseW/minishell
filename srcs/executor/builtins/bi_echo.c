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

#include <minishell.h>

int	check_n(char *cmd)
{
	int	idx;
	int	len;

	idx = 1;
	len = (int)ft_strlen(cmd);
	while (idx < len)
	{
		if (cmd[idx] != 'n')
			return (0);
		idx++;
	}
	return (SUCCESS);
}

void	echo_print(int idx, int flag_n, char **cmd)
{
	int	len;

	len = ft_strarrlen(cmd);
	while (idx < len)
	{
		ft_printf("%s", cmd[idx]);
		if (idx < len - 1)
			ft_printf(" ");
		idx++;
	}
	if (flag_n == 0)
		ft_printf("\n");
}

/*
** DESCRIPTION
**	-  The echo command writes its arguments separated by blanks and
**     terminated by a new line on the standard output. The -n option
**     prints a line without the newline;
**     echo without arguments prints only a new line.
*/

int	builtin_echo(char **cmd, t_exe *exe)
{
	int	idx;
	int	len;
	int	flag_n;

	(void)exe;
	idx = 1;
	flag_n = 0;
	len = ft_strarrlen(cmd);
	if (len > 1)
	{
		while (idx < len && ft_strnstr(cmd[idx], "-n", 2) != NULL)
		{
			flag_n = check_n(cmd[idx]);
			if (flag_n == SUCCESS)
				idx++;
			else
				break ;
		}
	}
	echo_print(idx, flag_n, cmd);
	return (EXIT_SUCCESS);
}
