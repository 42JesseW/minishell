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
	char		buff[PATH_MAX];
	const char	*env;
	int			exit_code;

	(void)cmd;
	(void)exe;
	exit_code = EXIT_SUCCESS;
	ft_bzero(buff, PATH_MAX);
	getcwd(buff, sizeof(buff));
	if (errno > 0)
	{
		env = environ_get(*exe->environ, "PWD");
		if (env)
			ft_strlcpy(buff, env, ft_strlen(env) + 1);
	}
	if (ft_strlen(buff) > 0)
		printf("%s\n", buff);
	else
	{
		exit_code = EXIT_FAILURE;
		ft_dprintf(STDERR_FILENO, "%s: pwd: %s\n", SHELL_NAME, strerror(errno));
		exit_code = EXIT_FAILURE;
	}
	errno = 0;
	return (exit_code);
}
