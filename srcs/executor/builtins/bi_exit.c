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

int	check_isdigit(char *str)
{
	int	idx;
	int	len;

	idx = 0;
	len = (int)ft_strlen(str);
	if (ft_strncmp(str, "-", 1) == 0)
		idx++;
	while (idx < len)
	{
		if (ft_isdigit(str[idx]) == 0)
			return (0);
		idx++;
	}
	return (SUCCESS);
}

int	calculate_error_code(char *cmd)
{
	int	exit_code;

	exit_code = ft_atoi(cmd);
	if (exit_code > 255)
		exit_code = exit_code % 256;
	return (exit_code);
}

int	builtin_exit(char **cmd, t_exe *exe)
{
	if (isatty(STDIN_FILENO))
		ft_printf("exit\n");
	errno = 0;
	if (ft_strarrlen(cmd) == 1)
	{
		*exe->exit_code = EXIT_SUCCESS;
		*exe->shell_exit = 1;
	}
	else if (ft_strarrlen(cmd) == 2 && check_isdigit(cmd[1]))
	{
		*exe->exit_code = calculate_error_code(cmd[1]);
		*exe->shell_exit = 1;
	}
	else if (ft_strarrlen(cmd) >= 2 && !check_isdigit(cmd[1]))
	{
		*exe->exit_code = 255;
		*exe->shell_exit = 1;
		ft_printf("%s: %s: %s: numeric argument required\n",
			SHELL_NAME, cmd[0], cmd[1]);
	}
	else
		ft_printf("%s: %s: too many arguments\n", SHELL_NAME, cmd[0]);
	return (SUCCESS);
}
