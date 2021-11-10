/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/05 16:37:36 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/05 16:37:36 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

#include <readline/readline.h>

int	main(int argc, char *argv[], char *envp[])
{
	t_env	*environ;

	environ = NULL;
	if (env_from_envp(&environ, (const char **) envp) == SYS_ERROR)
		return (EXIT_FAILURE);
	(void)argc;
	(void)argv;

	char	*line;

	while ((line = readline("> ")))
		printf("line: %s\n", line);
	return (EXIT_SUCCESS);
}
