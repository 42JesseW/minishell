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

static const char	prompt_startup[] = "\n"
	"        ______       ____         " B " .════. " R "    @   @\n"
	"       / __/ /  ___ / / /_ __   " B " / .═\"═.`. " R "   \\\\v/\n"
	"      _\\ \\/ _ \\/ -_) / / // /  " B "  ║ ║ '\\ \\ \\ " R "_/ )\n"
	"     /___/_//_/\\__/_/_/\\_, /  ,- " B "\\ `═.' /.'  / " R "\n"
	"                      /___/   '---`════'----'\n"
	"\n"
	"          -- created by aheister & jevan-de --"
	"\n";

/*
1. Discuss testing
 	- Github actions
 	- branches
	- unit tests
 	- regression test
2. Code reviews
 	- merge request
	- tiny features (pieces of functionality)
3. Minishell structure
 	- readline
 	- lexer / parser structure
 	- executor
*/

int	main(int argc, char *argv[], char *envp[])
{
	t_env	*environ;

	if (argc > 1)
		return (EXIT_SUCCESS);
	environ = NULL;
	printf("%s\n", prompt_startup);
	if (env_from_envp(&environ, (const char **) envp) == SYS_ERROR)
		return (EXIT_FAILURE);
	(void)argc;
	(void)argv;

	char	*line;

	while ((line = readline("> ")))
		printf("line: %s\n", line);
	return (EXIT_SUCCESS);
}
