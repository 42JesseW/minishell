/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_init.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/16 12:01:42 by aheister      #+#    #+#                 */
/*   Updated: 2021/12/16 12:01:42 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** DESCRIPTION
**	- initializes the builtin functions required by minishell. The function
**    reads from the array of builtins 'g_builtins' (each with the name and
**    the corresponding function of the builtin in a struct) and adds each
**    builtin to list exe->builtins.
** JOBS
** 1. Calculates the len of the array g_builtins
** 2. Creates in a loop a node from each builtin from g_builtins
** 3. 	by mallocing enough memory for a builtin
** 4.   copy the values from the array in the created builtin
** 5.   create a new t-list node from the builtin
** 6.   add it to the list exe->builtins
*/

static t_builtin	g_builtins[] = {
{"cd", &builtin_cd},
{"echo", &builtin_echo},
{"env", &builtin_env},
{"exit", NULL},
{"export", NULL},
{"pwd", &builtin_pwd},
{"unset", &builtin_unset},
};

t_builtin	*fill_builtin(int idx)
{
	t_builtin	*builtin;

	builtin = (t_builtin *)malloc(sizeof(t_builtin));
	if (!builtin)
	{
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "malloc",
			strerror(errno));
		return (NULL);
	}
	builtin->name = (g_builtins[idx]).name;
	builtin->function = (g_builtins[idx]).function;
	return (builtin);
}

int	init_builtins(t_exe *exe)
{
	int			idx;
	int			len;
	t_list		*node;
	t_builtin	*builtin;

	idx = 0;
	len = sizeof(g_builtins) / sizeof(t_builtin);
	while (idx < len)
	{
		builtin = fill_builtin(idx);
		node = ft_lstnew(builtin);
		if (node == NULL)
		{
			ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "malloc",
				strerror(errno));
			return (SYS_ERROR);
		}
		ft_lstadd_back(&exe->builtins, node);
		idx++;
	}
	return (SUCCESS);
}
