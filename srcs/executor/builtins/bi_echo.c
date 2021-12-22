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

/*
** DESCRIPTION
**	-  The echo command writes its arguments separated by blanks and
**     terminated by a new line on the standard output. The -n option
**     prints a line without the newline;
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

/*
** DESCRIPTION
**	-  The builtin echo writes its arguments separated by blanks and
**     terminated by a new line on the standard output. The -n option
**     prints a line without the newline;
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
	if (ft_strnstr(cmd[1], "-n", 2) != NULL) // TODO discuss (> 0) -> (!= NULL)
	{
		flag_n = check_n(cmd[1]);
		if (flag_n == SUCCESS)
			idx++;
	}
	while (idx < len)
	{
		printf("%s", cmd[idx]);
		if (idx < len - 1)
			printf(" ");
		idx++;
	}
	if (flag_n == 0)
		printf("\n");
	return (EXIT_SUCCESS);
}
