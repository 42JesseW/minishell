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

/* remove OLDPWD and increment SHLVL */
static int	init_environment(t_list **environ)
{
	const char	*old_val;
	char		*new_val;
	int			shell_level;

	old_val = environ_get(*environ, "SHLVL");
	if (!old_val)
		old_val = "1";
	shell_level = ft_atoi(old_val) + 1;
	new_val = ft_itoa(shell_level);
	if (!new_val || environ_update(environ, "SHLVL", new_val) == SYS_ERROR)
		return (SYS_ERROR);
	environ_remove(environ, "OLDPWD");
	free(new_val);
	return (SUCCESS);
}

/*
** shell_init has four jobs:
**	1. create a t_shell structure
**	2. initialize the t_shell structure
**	3. initialize the input_string string used by readline()
**	4. write an awesome ascii snail to STDOUT :)
*/

t_shell	*shell_init(const char *envp[])
{
	t_shell	*shell;
	t_list	*environ;

	init_inputrc();
	set_signals(true);
	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	environ = NULL;
	if (environ_from_envp(&environ, (const char **) envp) == SYS_ERROR
		|| init_environment(&environ) == SYS_ERROR)
	{
		shell_destroy(&shell);
		return (NULL);
	}
	shell->exit_code = 0;
	shell->environ = environ;
	shell->cmd_nodes = NULL;
	return (shell);
}
