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

static t_builtin 	g_builtins[] = {
		{"echo", &builtin_echo},
		{"cd", NULL},
		{NULL, NULL},
};

t_builtin *new_builtin(t_builtin old_builtin)
{
	t_builtin	*builtin;

	builtin = (t_builtin *)malloc(sizeof(t_builtin));
	if (!builtin)
		return (NULL);
	builtin->name = old_builtin.name; // TODO strdup toevoegen
	builtin->function = old_builtin.function;
	return(builtin);
}

int init_builtins(t_exe *exe)
{
	int 		idx;
	int			len;
	t_list		*l_builtin;
	t_builtin	*builtin;

	idx = 0;
	len = (sizeof(g_builtins) / sizeof(t_builtin));
	while (idx < len)
	{
		builtin = new_builtin(g_builtins[idx]);
		l_builtin = ft_lstnew(builtin);
		ft_lstadd_back(&exe->builtins, l_builtin);
		idx++;
	}
	return (SUCCESS);
}
