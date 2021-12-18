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

//static t_builtin 	g_builtins[] = {
//		{"echo", &builtin_echo},
//		{"cd", NULL},
//		{NULL, NULL},
//};

int init_builtins(t_exe *exe) // TODO uitbreiden naar meerdere builtins
{

	t_list		*l_builtin;
	t_builtin	*builtin;

	builtin = (t_builtin *)malloc(sizeof(t_builtin));
	if (!builtin)
		return (SYS_ERROR);
	builtin->name = "echo";
	builtin->function = &builtin_echo;
	l_builtin = ft_lstnew(builtin);					// TODO checken op malloc error
	ft_lstadd_back(&exe->builtins, l_builtin);
	return (SUCCESS);
}
