/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_init.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 11:15:14 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/12 11:15:14 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static const char	g_prompt_startup[] = "\n"
	"        ______       ____         " B " .════. " R "    @   @\n"
	"       / __/ /  ___ / / /_ __   " B " / .═\"═.`. " R "   \\\\v/\n"
	"      _\\ \\/ _ \\/ -_) / / // /  " B "  ║ ║ '\\ \\ \\ " R "_/ )\n"
	"     /___/_//_/\\__/_/_/\\_, /  ,- " B "\\ `═.' /.' " R " / \n"
	"                      /___/   '--- " B "`════' " R "----'\n"
	"\n"
	"          -- created by aheister & jevan-de --"
	"\n";

/*
** shell_init has four jobs:
**	1. create a t_shell structure
**	2. initialize the t_shell structure
**	3. initialize the input_string string used by readline()
**	4. write an awesome ascii snail to STDOUT :)
*/

t_shell	*shell_init(const char *envp[], char **input_string)
{
	t_shell	*shell;
	t_list	*environ;
	char	*line;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	environ = NULL;
	line = ft_strdup("");
	if (!line || environ_from_envp(&environ, (const char **) envp) == SYS_ERROR)
	{
		free(line);
		shell_destroy(&shell);
		return (NULL);
	}
	*input_string = line;
	shell->exit_code = 0;
	shell->environ = environ;
	shell->cmd_nodes = NULL;
	printf("%s\n", g_prompt_startup);
	return (shell);
}
